#pragma once

#include <memory>
#include <map>

#include <Tracy.hpp>

#include "Engine/ECS/System.hpp"

#include "Engine/Core/Resources/ResourceCache.hpp"
#include "Engine/Core/Resources/ResourceLoader.hpp"
#include "Engine/Common/Assert.hpp"
#include "Engine/Common/ProfilerSystems.hpp"

#include "Engine/Render/ECS/Components/RenderComponent.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/Render/ECS/RenderBatcher.hpp"

namespace SHV {
struct RenderComponent;

template <typename RenderBatchComponent>
class RenderSystem : public ::SHV::System {
   public:
    RenderSystem(entt::registry& registry,
                 std::unique_ptr<RenderBatcher> renderBatcher);
    virtual ~RenderSystem();

    virtual void Process(float dt);

    virtual void SetUp();
    virtual void TearDown();

   private:
    std::unique_ptr<RenderBatcher> renderBatcher;
};

template <typename RenderBatchComponent>
RenderSystem<RenderBatchComponent>::RenderSystem(
    entt::registry& registry, std::unique_ptr<RenderBatcher> aRenderBatcher)
    : System(registry), renderBatcher(std::move(aRenderBatcher)) {}

template <typename RenderBatchComponent>
RenderSystem<RenderBatchComponent>::~RenderSystem() = default;

template <typename RenderBatchComponent>
void RenderSystem<RenderBatchComponent>::Process(float /*dt*/) {
    ZoneNamedN(
        __tracy, "RenderSystem Process",
        static_cast<bool>(kActiveProfilerSystems & ProfilerSystems::ECS));
    
    bool renderBatchingHasStarted = false;

    auto renderView =
        registry.view<SHV::RenderComponent, SHV::TransformComponent>();

    for (auto&& [entity, renderComponent, transformComponent] :
         renderView.each()) {
        const auto* apiRenderComponent =
            registry.try_get<RenderBatchComponent>(entity);

        if (apiRenderComponent == nullptr) {
            if (!renderBatchingHasStarted) {
                renderBatchingHasStarted = true;
                renderBatcher->BeginRenderBatching();
            }
            
            // TODO: Add batch creation to a separate thread
            renderBatcher->AddRenderBatch(registry, entity, renderComponent);
        }
    }
    
    if (renderBatchingHasStarted) {
        renderBatcher->EndRenderBatching();
    }
}

template <typename RenderBatchComponent>
void RenderSystem<RenderBatchComponent>::SetUp() {
    AssertD(
        renderBatcher->IsRegistryEmptyFromBatchedRenderComponents(registry));
}

template <typename RenderBatchComponent>
void RenderSystem<RenderBatchComponent>::TearDown() {
    auto renderView = registry.view<RenderBatchComponent>();

    for (auto&& [entity, renderComponent] : renderView.each()) {
        renderComponent.renderBatch.Release();
    }

    registry.clear<RenderBatchComponent>();
}
}  // namespace SHV
