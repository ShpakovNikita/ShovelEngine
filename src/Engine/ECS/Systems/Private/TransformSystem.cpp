#include "Engine/ECS/Systems/TransformSystem.hpp"

#include <Tracy.hpp>
#include "Engine/Common/Assert.hpp"
#include "Engine/Common/ProfilerSystems.hpp"
#include "Engine/ECS/Entity.hpp"
#include "Engine/ECS/Components/RootComponent.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/ECS/Components/RelationshipComponent.hpp"

using namespace SHV;

void TransformSystem::Process(float /*dt*/) {
    ZoneNamedN(
        __tracy, "TransformSystem Process",
        static_cast<bool>(kActiveProfilerSystems & ProfilerSystems::ECS));

    auto rootComponentView = registry.view<RootComponent>();
    AssertD(rootComponentView.size() == 1);

    for (auto&& [sceneRoot] : rootComponentView.each()) {
        Entity::SceneGraphTraversal(
            registry, sceneRoot, [this](entt::entity& entity) {
                auto currentTransformComponent =
                    registry.try_get<TransformComponent>(entity);

                if (currentTransformComponent != nullptr) {
                    auto relationshipComponent =
                        registry.try_get<RelationshipComponent>(entity);

                    if (relationshipComponent->parent != entt::null) {
                        auto parentTransform = registry.get<TransformComponent>(
                            relationshipComponent->parent);
                        currentTransformComponent->parentWorldMatrix =
                            parentTransform.GetWorldMatrix();
                    }
                }
            });
    }
}