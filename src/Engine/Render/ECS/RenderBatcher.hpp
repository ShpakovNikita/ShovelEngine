#pragma once

#include <entt/entity/registry.hpp>

#include "Engine/Render/ECS/Components/RenderComponent.hpp"

namespace SHV {
class RenderBatcher {
   public:
    RenderBatcher() = default;
    virtual ~RenderBatcher() = default;
    
    virtual void BeginRenderBatching() {};
    virtual void EndRenderBatching() {};

    virtual void AddRenderBatch(
        entt::registry& registry, entt::entity& entity,
        const SHV::RenderComponent& renderComponent) = 0;

    virtual bool IsRegistryEmptyFromBatchedRenderComponents(
        entt::registry& registry) = 0;
};
}  // namespace SHV
