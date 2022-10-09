#pragma once

#include <entt/entity/registry.hpp>

#include "Engine/Render/ECS/RenderBatcher.hpp"

namespace SHV::Metal {
class LogicalDevice;
struct RenderComponent;

class RenderBatcher : public ::SHV::RenderBatcher {
   public:
    RenderBatcher(LogicalDevice& logicalDevice);
    virtual ~RenderBatcher() = default;

    virtual void AddRenderBatch(entt::registry& registry, entt::entity& entity,
                                const SHV::RenderComponent& renderComponent);

    virtual bool IsRegistryEmptyFromBatchedRenderComponents(
        entt::registry& registry);

   private:
    LogicalDevice& logicalDevice;
};
}  // namespace SHV::Metal