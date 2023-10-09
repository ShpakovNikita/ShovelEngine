#include "Engine/Render/Renderer.hpp"

#include "Engine/ECS/Scene.hpp"

#include "Engine/Render/ECS/Components/RenderComponent.hpp"
#include "Engine/Render/ECS/Components/SkyboxComponent.hpp"

#include <algorithm>

using namespace SHV;

SHV::Renderer::Renderer() = default;

SHV::Renderer::~Renderer() = default;

std::vector<entt::entity> SHV::Renderer::SortedRenderQueue(const Scene& scene) {
    std::vector<entt::entity> queue = {};
    entt::entity skybox = entt::null;
    scene.SceneGraphTraversal([&scene, &queue, &skybox](const entt::entity& entity) {
        auto skyboxComponent = scene.GetRegistry().try_get<SkyboxComponent>(entity);
        auto renderComponent = scene.GetRegistry().try_get<RenderComponent>(entity);
        if (renderComponent == nullptr || !renderComponent->isVisible) {
            return;
        }
        if (skyboxComponent != nullptr) {
            AssertD(skybox == entt::null);
            skybox = entity;
            return;
        }
        queue.push_back(entity);
    });
    if (skybox != entt::null) {
        queue.insert(queue.begin(), skybox);
    }
    return queue;
}
