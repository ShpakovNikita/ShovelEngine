#include "Engine/Render/Metal/ECS/RenderBatcher.hpp"

#include "Engine/Render/ECS/Components/RenderComponent.hpp"
#include "Engine/Render/Metal/ECS/Components/RenderComponent.hpp"
#include "Engine/Render/Metal/LogicalDevice.hpp"

using namespace SHV;

Metal::RenderBatcher::RenderBatcher(LogicalDevice& aLogicalDevice)
    : ::SHV::RenderBatcher(), logicalDevice(aLogicalDevice) {}

void Metal::RenderBatcher::AddRenderBatch(
    entt::registry& registry, entt::entity& entity,
    const SHV::RenderComponent& renderComponent) {
    auto& metalRenderComponent =
        registry.emplace<SHV::Metal::RenderComponent>(entity);
    metalRenderComponent.renderBatch =
        Metal::RenderBatch::Create(logicalDevice, renderComponent.primitive);
}

bool Metal::RenderBatcher::IsRegistryEmptyFromBatchedRenderComponents(
    entt::registry& registry) {
    auto renderView = registry.view<SHV::Metal::RenderComponent>();
    return renderView.size() == 0;
}