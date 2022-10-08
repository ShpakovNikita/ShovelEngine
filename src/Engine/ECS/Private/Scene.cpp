#include "Engine/ECS/Scene.hpp"

#include "Engine/ECS/Components/RelationshipComponent.hpp"

SHV::Scene::Scene() = default;
SHV::Scene::~Scene() = default;

void SHV::Scene::Process(float dt) {
    std::for_each(systems.begin(), systems.end(),
                  [dt](const auto& system) { system->Process(dt); });
}

void SHV::Scene::SetUp() {
    rootEntity = registry.create();
    registry.emplace<RelationshipComponent>(rootEntity);

    std::for_each(systems.begin(), systems.end(),
                  [](const auto& system) { system->SetUp(); });
}

void SHV::Scene::TearDown() {
    std::for_each(systems.begin(), systems.end(),
                  [](const auto& system) { system->TearDown(); });
}

entt::registry& SHV::Scene::GetRegistry() { return registry; }
const entt::registry& SHV::Scene::GetRegistry() const { return registry; }

entt::entity& SHV::Scene::GetRootEntity() { return rootEntity; }
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