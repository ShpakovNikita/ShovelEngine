#include "Engine/Render/ECS/Systems/SkyboxSystem.hpp"

#include <Tracy.hpp>
#include "Engine/Common/ProfilerSystems.hpp"

#include "Engine/Render/ECS/Components/SkyboxComponent.hpp"
#include "Engine/Render/ECS/Components/RenderComponent.hpp"
#include "Engine/Render/RenderContext.hpp"
#include "Engine/Render/Model/ShaderParamNames.hpp"

#include "Engine/ECS/Utils/ObjectsCreationUtils.hpp"
#include "Engine/ECS/Scene.hpp"
#include "Engine/ECS/Entity.hpp"

#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/ECS/Components/CameraComponent.hpp"

#include <vector>

using namespace SHV;

namespace SSkyboxSystem {
    std::unique_ptr<Scene> CreateSkyboxScene(std::shared_ptr<Texture> equirectangularTexture) {
        auto scene = std::make_unique<Scene>();

        auto& registry = scene->GetRegistry();
        auto camera = registry.create();

        registry.emplace<TransformComponent>(camera);
        registry.emplace<CameraComponent>(camera);

        auto cubeEntity = ObjectCreationUtils::CreateCube(scene->GetRegistry());

        auto& renderComponent = registry.get<RenderComponent>(cubeEntity);
        renderComponent.material.materialShader = eMaterialShader::kSkyboxEquirectangularShader;
        renderComponent.material.textures[SHV::SkyboxParams::kSkyboxMap] = equirectangularTexture;

        Entity::AddChild(registry, scene->GetRootEntity(), camera);
        Entity::AddChild(registry, scene->GetRootEntity(), cubeEntity);

        return scene;
    }
}

SkyboxSystem::SkyboxSystem(entt::registry& registry, RenderContext& aRenderContext)
    : System(registry), renderContext(aRenderContext) {}

SkyboxSystem::~SkyboxSystem() = default;

void SkyboxSystem::Process(float /*dt*/) {
    ZoneNamedN(
        __tracy, "SkyboxSystem Process",
        static_cast<bool>(kActiveProfilerSystems & ProfilerSystems::ECS));

    auto renderView = registry.view<SkyboxComponent>();
    
    for (auto&& [entity, skyboxComponent] : renderView.each()) {
        auto renderComponent = registry.try_get<RenderComponent>(entity);
        if (renderComponent == nullptr) {
            CreateSkyboxCubeMap(entity);
        }
    }
    
    renderContext.GetWindow();
}

void SkyboxSystem::CreateSkyboxCubeMap(entt::entity entity) {
    auto& component = registry.get<SkyboxComponent>(entity);
    std::shared_ptr<Texture> texture = CreateCubeMapTextureFromEquirectangularProjection(component.equirectangularTexture);
    Primitive cubePrimitive = ObjectCreationUtils::CreateCubePrimitive();

    auto& renderComponent = registry.emplace<RenderComponent>(entity);
    renderComponent.material.materialShader = eMaterialShader::kSkyboxShader;
    renderComponent.material.textures[SHV::SkyboxParams::kSkyboxMap] = texture;
    renderComponent.primitive = cubePrimitive;
    component.equirectangularTexture = nullptr;
}

std::shared_ptr<Texture> SkyboxSystem::CreateCubeMapTextureFromEquirectangularProjection(std::shared_ptr<Texture> equirectangularTexture) {
    std::unique_ptr<Scene> scene = SSkyboxSystem::CreateSkyboxScene(equirectangularTexture);
    Renderer& renderer = renderContext.GetRenderer();
    renderer.SetUpScene(*scene);

    // Trigger creation of api specific render batches
    scene->Process(0.0);

    static std::vector<glm::vec3> orientations = {
        {1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0}, {0.0, -1.0, 0.0},
        {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0},
    };

    std::vector<std::shared_ptr<Texture>> sides = {};
    sides.reserve(orientations.capacity());
    uint32_t targetSize = equirectangularTexture->GetHeight() / 2;

    for (const glm::vec3& orientation: orientations) {
        auto texture = std::make_shared<Texture>(nullptr, targetSize, targetSize,
                                                  4, 1);
        auto cameraEntity = scene->GetEntityWithActiveCamera();
        const auto& cameraComponent =
            scene->GetRegistry().get<SHV::CameraComponent>(cameraEntity);
        auto& cameraTransform =
            scene->GetRegistry().get<SHV::TransformComponent>(cameraEntity);
        cameraTransform.rotation = glm::quatLookAt(orientation, cameraComponent.cameraUp);

        texture = renderer.Draw(*scene, *texture);
        sides.push_back(texture);
    }

    renderer.TearDownScene(*scene);
    return std::make_shared<Texture>(sides);
}