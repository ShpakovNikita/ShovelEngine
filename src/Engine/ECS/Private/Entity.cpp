#include "Engine/ECS/Entity.hpp"

#include "Engine/ECS/Components/TransformComponent.hpp"

#include <entt/entt.hpp>

using namespace SHV;

void Entity::AddChild(entt::registry& registry, entt::entity& parent,
                      entt::entity& child) {
    auto& parentRelationship =
        registry.get_or_emplace<RelationshipComponent>(parent);
    auto& parentTransformComponent =
        registry.get_or_emplace<TransformComponent>(parent);

    auto& childRelationship =
        registry.get_or_emplace<RelationshipComponent>(child);
    auto& childTransformComponent =
        registry.get_or_emplace<TransformComponent>(child);

    childRelationship.parent = parent;
    childTransformComponent.parentWorldMatrix =
        parentTransformComponent.GetWorldMatrix();

    if (parentRelationship.first == entt::null) {
        parentRelationship.first = child;
    } else {
        auto& currRelationship =
            registry.get<RelationshipComponent>(parentRelationship.first);
        while (currRelationship.next != entt::null) {
            currRelationship =
                registry.get<RelationshipComponent>(currRelationship.next);
        }

        auto currEntity = entt::to_entity(registry, currRelationship);

        currRelationship.next = child;
        childRelationship.prev = currEntity;
    }
}

bool Entity::IsNodesConnected(const entt::registry& registry,
                              const entt::entity& rootNode,
                              const entt::entity& childNode) {
    auto relationshipComponent =
        registry.try_get<RelationshipComponent>(childNode);
    while (relationshipComponent != nullptr &&
           relationshipComponent->parent != entt::null) {
        if (relationshipComponent->parent == rootNode) {
            return true;
        }
        relationshipComponent = registry.try_get<RelationshipComponent>(
            relationshipComponent->parent);
    }

    return false;
}