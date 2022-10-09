#pragma once

#include <entt/entity/registry.hpp>

#include "Engine/Render/ECS/Components/RenderComponent.hpp"
#include "Engine/Render/ECS/RenderBatcher.hpp"

namespace SHV::OpenGl {
class RenderBatcher : public ::SHV::RenderBatcher {
   public:
    RenderBatcher() = default;
    virtual ~RenderBatcher() = default;

    virtual void AddRenderBatch(entt::registry& registry, entt::entity& entity,
                                const SHV::RenderComponent& renderComponent);

    virtual bool IsRegistryEmptyFromBatchedRenderComponents(
        entt::registry& registry);
};
}  // namespace SHV::OpenGl