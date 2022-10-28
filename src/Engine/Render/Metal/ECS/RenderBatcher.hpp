#pragma once

#include <entt/entity/registry.hpp>

#include <map>

#include "Engine/Render/ECS/RenderBatcher.hpp"

namespace MTL {
class CommandBuffer;
class BlitCommandEncoder;
}  // namespace MTL

namespace SHV {

class Texture;

namespace Metal {
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
    std::map<Texture*, std::shared_ptr<GPUTexture>> gpuTexturesCache;
    std::unique_ptr<RenderPipelineCache> pipelineCache;

    MTL::CommandBuffer* resourcesCommandBuffer = nullptr;
    MTL::BlitCommandEncoder* blitCommandEncoder = nullptr;
};
}  // namespace Metal
}  // namespace SHV
