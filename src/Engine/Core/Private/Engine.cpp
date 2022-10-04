#include "Engine/Core/Engine.hpp"

#include <SDL.h>

#include <chrono>
#include <thread>

#include <imgui.h>

#include "Engine/Common/Exception.hpp"
#include "Engine/Common/Logger.hpp"
#include "Engine/Core/ImmutableConfig.hpp"
#include "Engine/Core/MutableConfig.hpp"
#include "Engine/Render/ImGuiImpl.hpp"
#include "Engine/Render/RenderContext.hpp"
#include "Engine/Tools/Toolbar.hpp"
#include "Engine/ECS/Scene.hpp"

using namespace SHV;

Engine::Engine(const ImmutableConfig& aImmutableConfig,
               MutableConfig& aMutableConfig)
    : immutableConfig(aImmutableConfig),
      mutableConfig(aMutableConfig),
      renderContext(nullptr) {}

int Engine::Run() noexcept {
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

    const WindowConfig windowConfig = {
        immutableConfig.width, immutableConfig.height,
        immutableConfig.windowName, immutableConfig.renderApi};
    renderContext = std::make_unique<RenderContext>(windowConfig,
                                                    immutableConfig.renderApi);
    renderContext->SetUp();

    imgui = std::make_unique<ImGui>(*renderContext);
    imgui->SetUp();

    scene = std::make_unique<Scene>();
    toolbar = std::make_unique<Toolbar>(*scene);
}

void Engine::TearDown() {
    scene = nullptr;
    toolbar = nullptr;

    imgui->TearDown();
    imgui = nullptr;

    renderContext->TearDown();
    renderContext = nullptr;

    SDL_Quit();
}

void Engine::MainLoop() {
    isRunning = true;
    const float minSecPerFrame = 1.0f / mutableConfig.fpsLimit;

    LogI(eTag::kEngine) << "Engine starting main loop" << std::endl;

    auto previousTime = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::microseconds(
        static_cast<size_t>(minSecPerFrame * 1000.0f)));

    while (isRunning) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime = currentTime - previousTime;
        float deltaTime =
            std::chrono::duration_cast<std::chrono::nanoseconds>(elapsedTime)
                .count() /
            (1000.0f * 1000.0f * 1000.0f);

        Tick(deltaTime);

        std::chrono::milliseconds timeToSleep(std::max(
            0, static_cast<int>((minSecPerFrame - deltaTime) * 1000.0f)));
        std::this_thread::sleep_for(timeToSleep);

        previousTime = currentTime;
    }

    LogI(eTag::kEngine) << "Engine ended main loop" << std::endl;
}

void Engine::Tick(float deltaTime) {
    PollEvents(deltaTime);
    UpdateSystems(deltaTime);
    RenderLoop(deltaTime);
}

void Engine::PollEvents(float /* deltaTime */) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        imgui->PollEvents(&e);
        if (e.type == SDL_QUIT) isRunning = false;
    }
}

void Engine::UpdateSystems(float /* deltaTime */) {}

void Engine::RenderLoop(float /* deltaTime */) {
    renderContext->GetRenderer().BeginFrame();
    imgui->BeginFrame();

    renderContext->GetRenderer().Draw(*scene);
    toolbar->Draw();

    imgui->EndFrame();
    renderContext->GetRenderer().EndFrame();
}

Engine::~Engine() = default;
