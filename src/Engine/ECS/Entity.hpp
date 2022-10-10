#pragma once

#include <entt/entity/registry.hpp>

#include <functional>

#include "Engine/ECS/Components/RelationshipComponent.hpp"

namespace SHV {
namespace Entity {
void AddChild(entt::registry& registry, entt::entity& parent,
              entt::entity& child);

void SceneGraphTraversal(
    entt::registry& registry, entt::entity& entity,
    const std::function<void(entt::entity&)>& entityProcessor);

template <typename Component>
Component* GetFirstComponentInHierarchy(entt::registry& registry,
                                        entt::entity& currentNode);

template <typename Component>
const Component* GetFirstComponentInHierarchy(const entt::registry& registry,
                                              const entt::entity& currentNode);
}  // namespace Entity
}  // namespace SHV

template <typename Component>
const Component* SHV::Entity::GetFirstComponentInHierarchy(
    const entt::registry& registry, const entt::entity& entity) {
    auto* component = registry.try_get<Component>(entity);
    entt::entity currNode = entity;

    while (component == nullptr && currNode != entt::null) {
        currNode = registry.get<RelationshipComponent>(currNode).parent;
        component = registry.try_get<Component>(currNode);
    }

    return component;
}

template <typename Component>
Component* SHV::Entity::GetFirstComponentInHierarchy(entt::registry& registry,
                                                     entt::entity& entity) {
    return const_cast<Component*>(
        GetFirstComponentInHierarchy<Component>(registry, entity));
}