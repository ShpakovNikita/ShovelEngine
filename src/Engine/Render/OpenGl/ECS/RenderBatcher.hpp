#pragma once

#include <entt/entity/registry.hpp>

#include "Engine/Render/ECS/Components/RenderComponent.hpp"
#include "Engine/Render/ECS/RenderBatcher.hpp"

#include "Engine/Core/Resources/ResourceCache.hpp"
#include "Engine/Core/Resources/ResourceLoader.hpp"

namespace SHV::OpenGl {
class ShaderCache;
class GPUTexture;

class RenderBatcher : public ::SHV::RenderBatcher {
   public:
    RenderBatcher();
    virtual ~RenderBatcher() = default;

    virtual void AddRenderBatch(entt::registry& registry, entt::entity& entity,
                                const SHV::RenderComponent& renderComponent);

    virtual bool IsRegistryEmptyFromBatchedRenderComponents(
        entt::registry& registry);

   private:
    std::map<Texture*, std::shared_ptr<GPUTexture>> gpuTexturesCache;
    std::unique_ptr<ShaderCache> shaderCache;
};
}  // namespace SHV::OpenGl