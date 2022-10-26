#include "Engine/Render/Metal/ECS/RenderBatcher.hpp"

#include "Engine/Render/ECS/Components/RenderComponent.hpp"
#include "Engine/Render/Metal/ECS/Components/RenderComponent.hpp"
#include "Engine/Render/Metal/LogicalDevice.hpp"
#include "Engine/Render/Metal/Model/GPUTexture.hpp"
#include "Engine/Render/Metal/Model/RenderMaterial.hpp"
#include "Engine/Render/Metal/RenderPipelineCache.hpp"
#include "Engine/Render/Metal/CommandQueue.hpp"

#include "Metal/Metal.hpp"

using namespace SHV;

Metal::RenderBatcher::RenderBatcher(LogicalDevice& aLogicalDevice,
                                    CommandQueue& aCommandQueue)
    : ::SHV::RenderBatcher(),
      logicalDevice(aLogicalDevice),
      commandQueue(aCommandQueue),
      gpuTexturesCache(std::make_unique<ResourceCache<GPUTexture>>()),
      pipelineCache(std::make_unique<RenderPipelineCache>(aLogicalDevice)) {}

void Metal::RenderBatcher::BeginRenderBatching() {
    resourcesCommandBuffer = commandQueue.GetCommandQueue().commandBuffer();
    blitCommandEncoder = resourcesCommandBuffer->blitCommandEncoder();
}

void Metal::RenderBatcher::EndRenderBatching() {
    blitCommandEncoder->endEncoding();
    blitCommandEncoder->release();
    blitCommandEncoder = nullptr;

    resourcesCommandBuffer->commit();
    resourcesCommandBuffer->release();
    resourcesCommandBuffer = nullptr;
}

void Metal::RenderBatcher::AddRenderBatch(
    entt::registry& registry, entt::entity& entity,
    const SHV::RenderComponent& renderComponent) {
    auto& metalRenderComponent =
        registry.emplace<SHV::Metal::RenderComponent>(entity);
    metalRenderComponent.renderBatch =
        Metal::RenderBatch::Create(logicalDevice, renderComponent.primitive);

    metalRenderComponent.renderMaterial = std::make_shared<RenderMaterial>(
        pipelineCache->Get(renderComponent.material.materialShader));

    for (auto& [param, texture] : renderComponent.material.textures) {
        auto gpuTexture = gpuTexturesCache->Get(
            texture->GetTexturePath(), logicalDevice, *blitCommandEncoder);
        metalRenderComponent.renderMaterial->SetTexture(param, gpuTexture);
    }
}

bool Metal::RenderBatcher::IsRegistryEmptyFromBatchedRenderComponents(
    entt::registry& registry) {
    auto renderView = registry.view<SHV::Metal::RenderComponent>();
    return renderView.size() == 0;
}
