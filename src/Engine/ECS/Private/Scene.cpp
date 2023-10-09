#include "Engine/ECS/Scene.hpp"

#include "Engine/ECS/Components/RelationshipComponent.hpp"
#include "Engine/ECS/Components/CameraComponent.hpp"
#include "Engine/ECS/Components/RootComponent.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/ECS/Entity.hpp"

SHV::Scene::Scene() {
    rootEntity = registry.create();
    registry.emplace<TransformComponent>(rootEntity);
    registry.emplace<RootComponent>(rootEntity);
};
SHV::Scene::~Scene() = default;

void SHV::Scene::Process(float dt) {
    AssertD(registry.view<RootComponent>().size() == 1);

    std::for_each(systems.begin(), systems.end(),
                  [dt](const auto& system) { system->Process(dt); });
}

entt::registry& SHV::Scene::GetRegistry() { return registry; }
const entt::registry& SHV::Scene::GetRegistry() const { return registry; }

entt::entity& SHV::Scene::GetRootEntity() { return rootEntity; }
const entt::entity& SHV::Scene::GetRootEntity() const { return rootEntity; }

void SHV::Scene::SceneGraphTraversal(
    const std::function<void(entt::entity&)>& entityProcessor) {
    Entity::SceneGraphTraversal(registry, rootEntity, entityProcessor);
}

void SHV::Scene::SceneGraphTraversal(
    const std::function<void(const entt::entity&)>& entityProcessor) const {
    Entity::SceneGraphTraversal(registry, rootEntity, entityProcessor);
}

entt::entity SHV::Scene::GetEntityWithActiveCamera() const {
    auto renderView = registry.view<SHV::CameraComponent>();

    for (auto&& [entity, cameraComponent] : renderView.each()) {
        if (cameraComponent.isActive) {
            return entity;
        }
    }

    return entt::null;
}