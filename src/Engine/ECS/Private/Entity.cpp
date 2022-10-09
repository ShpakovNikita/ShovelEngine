#include "Engine/ECS/Entity.hpp"

#include <entt/entt.hpp>

using namespace SHV;

void Entity::AddChild(entt::registry& registry, entt::entity& parent,
                      entt::entity& child) {
    auto& parentRelationship =
        registry.get_or_emplace<RelationshipComponent>(parent);
    auto& childRelationship =
        registry.get_or_emplace<RelationshipComponent>(child);

    childRelationship.parent = parent;

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