#include "Engine/ECS/Scene.hpp"

#include "Engine/ECS/Components/RelationshipComponent.hpp"

SHV::Scene::Scene() = default;
SHV::Scene::~Scene() = default;

const entt::registry& SHV::Scene::GetRegistry() const { return registry; }

const entt::entity& SHV::Scene::GetRootEntity() const { return rootEntity; }

void SHV::Scene::SceneGraphTraversal(
    const std::function<void(entt::entity&)>& entityProcessor) {
    SceneGraphTraversal(rootEntity, entityProcessor);
}

void SHV::Scene::SceneGraphTraversal(
    entt::entity& entity,
    const std::function<void(entt::entity&)>& entityProcessor) {
    auto curr = entity;

    while (curr != entt::null) {
        entityProcessor(curr);
        SceneGraphTraversal(registry.get<RelationshipComponent>(curr).first,
                            entityProcessor);
        curr = registry.get<RelationshipComponent>(curr).next;
    }
}