#include "Engine/Core/Engine.hpp"

#include <glm/glm.hpp>

#include <SDL.h>

#include <chrono>
#include <thread>

#include <imgui.h>
#include <TracySystem.hpp>
#include <Tracy.hpp>

#include "Engine/Common/Exception.hpp"
#include "Engine/Common/Logger.hpp"

#include "Engine/Core/ImmutableConfig.hpp"
#include "Engine/Core/MutableConfig.hpp"
#include "Engine/Core/Window.hpp"
#include "Engine/Core/FileSystem.hpp"
#include "Engine/Core/Input/InputManager.hpp"
#include "Engine/Core/Resources/ResourceManager.hpp"

#include "Engine/Render/ImGuiImpl.hpp"
#include "Engine/Render/RenderContext.hpp"
#include "Engine/Render/Model/Material.hpp"
#include "Engine/Render/Model/ShaderParamNames.hpp"
#include "Engine/Render/ECS/Components/RenderComponent.hpp"
#include "Engine/Render/ECS/Components/SkyboxComponent.hpp"
#include "Engine/Render/ECS/Systems/SkyboxSystem.hpp"

#include "Engine/Tools/Toolbar.hpp"

#include "Engine/ECS/Scene.hpp"
#include "Engine/ECS/Entity.hpp"
#include "Engine/ECS/Systems/CameraSystem.hpp"
#include "Engine/ECS/Systems/InputSystem.hpp"
#include "Engine/ECS/Systems/MovementSystem.hpp"
#include "Engine/ECS/Systems/TransformSystem.hpp"

#include "Engine/ECS/Utils/ObjectsCreationUtils.hpp"
#include "Engine/ECS/Utils/AspectRatioDelegate/WindowAspectRatioDelegate.hpp"

#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/ECS/Components/MovementComponent.hpp"
#include "Engine/ECS/Components/CameraComponent.hpp"
#include "Engine/ECS/Components/InputComponent.hpp"
#include "Engine/ECS/Components/NameComponent.hpp"

using namespace SHV;

Engine* enginePtr = nullptr;

Engine::Engine(const ImmutableConfig& aImmutableConfig,
               MutableConfig& aMutableConfig)
    : immutableConfig(aImmutableConfig),
      mutableConfig(aMutableConfig),
      renderContext(nullptr) {
    AssertD(enginePtr == nullptr);
    enginePtr = this;
}

Engine::~Engine() {
    AssertD(enginePtr != nullptr);
    enginePtr = nullptr;
};

Engine& Engine::Get() {
    AssertD(enginePtr != nullptr);
    return *enginePtr;
}

int Engine::Run() noexcept {
    tracy::SetThreadName("main");

    LogI(eTag::kEngine) << "Engine setup begin" << std::endl;
    try {
        SetUp();
    } catch (SHV::Exception& e) {
        SHV::LogE(SHV::eTag::kBase)
            << "Critical error occurred during engine SetUp: " << e.what();
        return EXIT_FAILURE;
    }
    LogI(eTag::kEngine) << "Engine setup complete" << std::endl;

    // Executes until some command to close engine
    MainLoop();

    LogI(eTag::kEngine) << "Engine tear down begin" << std::endl;
    try {
        TearDown();
    } catch (SHV::Exception& e) {
        SHV::LogE(SHV::eTag::kBase)
            << "Critical error occurred during engine TearDown: " << e.what();
        return EXIT_FAILURE;
    }
    LogI(eTag::kEngine) << "Engine tear down complete" << std::endl;

    return EXIT_SUCCESS;
}

void Engine::SetUp() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw Exception("SDL could not initialize! SDL_Error: %s\n",
                        SDL_GetError());
    }

    fileSystem = std::make_unique<FileSystemImpl>();
    resourceManager = std::make_unique<ResourceManager>();

    const WindowConfig windowConfig = {immutableConfig.width,
                                       immutableConfig.height,
                                       immutableConfig.windowName};

    renderContext =
        std::make_unique<RenderContext>(windowConfig, mutableConfig.renderApi);
    renderContext->SetUp();

    CreateScene();
    CreateCharacter();

    toolbar = std::make_unique<Toolbar>(*scene, *this);
}

void Engine::TearDown() {
    DestroyScene();

    toolbar = nullptr;

    renderContext->TearDown();
    renderContext = nullptr;

    resourceManager = nullptr;
    fileSystem = nullptr;

    SDL_Quit();
}

void Engine::MainLoop() {
    isRunning = true;
    const float minSecPerFrame = 1.0f / mutableConfig.fpsLimit;

    LogI(eTag::kEngine) << "Engine starting main loop" << std::endl;

    auto previousTime = std::chrono::high_resolution_clock::now();

    while (isRunning) {
        {
            ZoneScopedN("Frame Execution");

            auto currentTime = std::chrono::high_resolution_clock::now();
            auto elapsedTime = currentTime - previousTime;
            float deltaTime =
                std::chrono::duration_cast<std::chrono::nanoseconds>(
                    elapsedTime)
                    .count() /
                (1000.0f * 1000.0f * 1000.0f);

            Tick(deltaTime);

            std::chrono::milliseconds timeToSleep(std::max(
                0, static_cast<int>((minSecPerFrame - deltaTime) * 1000.0f)));
            std::this_thread::sleep_for(timeToSleep);

            previousTime = currentTime;
        }
        FrameMark;
    }

    LogI(eTag::kEngine) << "Engine ended main loop" << std::endl;
}

void Engine::Tick(float deltaTime) {
    {
        ZoneScopedN("Handle Mutable Config Change");
        HandleMutableConfigChange();
    }

    {
        ZoneScopedN("Poll Events");
        PollEvents(deltaTime);
    }

    {
        ZoneScopedN("Update Systems");
        UpdateSystems(deltaTime);
    }

    {
        ZoneScopedN("Render Frame");
        RenderLoop(deltaTime);
    }
}

void Engine::HandleMutableConfigChange() {
    if (newMutableConfig) {
        // Switch render API
        if (mutableConfig.renderApi != newMutableConfig.value().renderApi) {
            renderContext->GetRenderer().TearDownScene(*scene);

            renderContext->SwitchRenderApi(newMutableConfig.value().renderApi);

            renderContext->GetRenderer().SetUpScene(*scene);
        }

        mutableConfig = newMutableConfig.value();
        newMutableConfig = std::nullopt;
    }
}

void Engine::PollEvents(float /* deltaTime */) {
    SDL_Event e;
    renderContext->GetWindow().GetInputManager().ClearInput();

    while (SDL_PollEvent(&e)) {
        renderContext->GetImGui().PollEvents(&e);
        if ((e.type == SDL_KEYDOWN || e.type == SDL_KEYUP ||
             e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN ||
             e.type == SDL_MOUSEBUTTONUP) &&
            e.key.windowID == renderContext->GetWindow().GetWindowID()) {
            renderContext->GetWindow().GetInputManager().PollEvents(&e);
        }
        if (e.type == SDL_QUIT) isRunning = false;
    }
}

void Engine::UpdateSystems(float deltaTime) { scene->Process(deltaTime); }

void Engine::RenderLoop(float /* deltaTime */) {
    renderContext->GetRenderer().BeginFrame();
    renderContext->GetImGui().BeginFrame();

    renderContext->GetRenderer().Draw(*scene);

    toolbar->Draw();

    renderContext->GetImGui().EndFrame();
    renderContext->GetRenderer().EndFrame();

    renderContext->GetRenderer().WaitForFrameExecutionFinish();
}

const MutableConfig& Engine::GetMutableConfig() const { return mutableConfig; }

void Engine::SetMutableConfig(MutableConfig& config) {
    newMutableConfig = config;
}

void Engine::LoadPrimitives() {
    auto& registry = scene->GetRegistry();

    auto cubeEntity = ObjectCreationUtils::CreateCube(registry);
    auto& renderComponent = registry.get<RenderComponent>(cubeEntity);
    renderComponent.material.materialShader = eMaterialShader::kPbrShader;
    renderComponent.material.textures[SHV::PbrParams::kAlbedoMap] =
        resourceManager->Get<Texture>("sloppy-mortar-bricks_albedo.png");
    renderComponent.material.textures[SHV::PbrParams::kAoMap] =
        resourceManager->Get<Texture>("sloppy-mortar-bricks_ao.png");
    renderComponent.material.textures[SHV::PbrParams::kHeightMap] =
        resourceManager->Get<Texture>("sloppy-mortar-bricks_height.png");
    renderComponent.material.textures[SHV::PbrParams::kMetallicRoughnessMap] =
        resourceManager->Get<Texture>("sloppy-mortar-bricks_metallic.png");
    renderComponent.material.textures[SHV::PbrParams::kNormalMap] =
        resourceManager->Get<Texture>("sloppy-mortar-bricks_normal-ogl.png");

    auto& nameComponent = registry.emplace<SHV::NameComponent>(cubeEntity);
    nameComponent.name = "Cube";

    auto skyboxEntity = registry.create();
    auto& skyboxComponent = registry.emplace<SkyboxComponent>(skyboxEntity);
    auto& skyboxNameComponent = registry.emplace<SHV::NameComponent>(skyboxEntity);
    skyboxNameComponent.name = "Skybox";

    skyboxComponent.equirectangularTexture = resourceManager->Get<Texture>("small_empty_room_1_4k.hdr");

    Entity::AddChild(registry, scene->GetRootEntity(), skyboxEntity);
    Entity::AddChild(registry, scene->GetRootEntity(), cubeEntity);
}

void Engine::UnloadPrimitives() {}

void Engine::CreateScene() {
    scene = std::make_unique<Scene>();
    scene->AddSystem<InputSystem>(renderContext->GetWindow().GetInputManager(),
                                  renderContext->GetWindow());
    scene->AddSystem<MovementSystem>();
    scene->AddSystem<TransformSystem>();
    scene->AddSystem<CameraSystem>(std::make_unique<WindowAspectRatioDelegate>(renderContext->GetWindow()));
    scene->AddSystem<SkyboxSystem>(*renderContext);

    renderContext->GetRenderer().SetUpScene(*scene);

    LoadPrimitives();
}

void Engine::DestroyScene() {
    UnloadPrimitives();

    renderContext->GetRenderer().TearDownScene(*scene);

    scene->RemoveSystem<SkyboxSystem>();
    scene->RemoveSystem<CameraSystem>();
    scene->RemoveSystem<TransformSystem>();
    scene->RemoveSystem<MovementSystem>();
    scene->RemoveSystem<InputSystem>();
    scene = nullptr;
}

void Engine::CreateCharacter() {
    auto& registry = scene->GetRegistry();
    auto character = registry.create();

    registry.emplace<TransformComponent>(character);
    registry.emplace<InputComponent>(character);
    registry.emplace<MovementComponent>(character);
    registry.emplace<CameraComponent>(character);
    auto& nameComponent = registry.emplace<NameComponent>(character);
    nameComponent.name = "Camera";

    Entity::AddChild(registry, scene->GetRootEntity(), character);
}

const FileSystem& Engine::GetFileSystem() const { return *fileSystem; }

ResourceManager& Engine::GetResourceManager() const { return *resourceManager; }
