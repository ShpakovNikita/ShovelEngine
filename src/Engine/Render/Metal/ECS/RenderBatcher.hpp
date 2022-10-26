#pragma once

#include <entt/entity/registry.hpp>

#include "Engine/Render/ECS/RenderBatcher.hpp"

#include "Engine/Core/Resources/ResourceCache.hpp"

namespace MTL {
class CommandBuffer;
class BlitCommandEncoder;
}  // namespace MTL

namespace SHV::Metal {
class LogicalDevice;
class GPUTexture;
class RenderPipelineCache;
class CommandQueue;
struct RenderComponent;

class RenderBatcher : public ::SHV::RenderBatcher {
   public:
    RenderBatcher(LogicalDevice& logicalDevice, CommandQueue& commandQueue);
    virtual ~RenderBatcher() = default;

    virtual void BeginRenderBatching();
    virtual void EndRenderBatching();

    virtual void AddRenderBatch(entt::registry& registry, entt::entity& entity,
                                const SHV::RenderComponent& renderComponent);

    virtual bool IsRegistryEmptyFromBatchedRenderComponents(
        entt::registry& registry);

   private:
    LogicalDevice& logicalDevice;
    CommandQueue& commandQueue;
    std::unique_ptr<ResourceCache<GPUTexture>> gpuTexturesCache;
    std::unique_ptr<RenderPipelineCache> pipelineCache;

    MTL::CommandBuffer* resourcesCommandBuffer = nullptr;
    MTL::BlitCommandEncoder* blitCommandEncoder = nullptr;
};
}  // namespace SHV::Metal
