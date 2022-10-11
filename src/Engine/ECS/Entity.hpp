#pragma once

#include <entt/entity/registry.hpp>

#include <functional>

#include "Engine/ECS/Components/RelationshipComponent.hpp"

namespace SHV {
namespace Entity {
void AddChild(entt::registry& registry, entt::entity& parent,
              entt::entity& child);

template <typename ProcessorFunc>
void SceneGraphTraversal(entt::registry& registry, entt::entity& entity,
                         const ProcessorFunc& entityProcessor);

template <typename ProcessorFunc>
void SceneGraphTraversal(const entt::registry& registry,
                         const entt::entity& entity,
                         const ProcessorFunc& entityProcessor);

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

template <typename ProcessorFunc>
void SHV::Entity::SceneGraphTraversal(const entt::registry& registry,
                                      const entt::entity& entity,
                                      const ProcessorFunc& entityProcessor) {
    return SceneGraphTraversal(const_cast<entt::registry&>(registry),
                               const_cast<entt::entity&>(entity),
                               entityProcessor);
}

template <typename ProcessorFunc>
void SHV::Entity::SceneGraphTraversal(entt::registry& registry,
                                      entt::entity& entity,
                                      const ProcessorFunc& entityProcessor) {
    auto curr = entity;

    while (curr != entt::null) {
        entityProcessor(curr);
        SceneGraphTraversal(registry,
                            registry.get<RelationshipComponent>(curr).first,
                            entityProcessor);
        curr = registry.get<RelationshipComponent>(curr).next;
    }
}