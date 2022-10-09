#include "Engine/Render/OpenGl/ECS/RenderBatcher.hpp"

#include "Engine/Render/OpenGl/ECS/Components/RenderComponent.hpp"

using namespace SHV;

void OpenGl::RenderBatcher::AddRenderBatch(
    entt::registry& registry, entt::entity& entity,
    const SHV::RenderComponent& renderComponent) {
    auto& openGlRenderComponent =
        registry.emplace<SHV::OpenGl::RenderComponent>(entity);
    openGlRenderComponent.renderBatch =
        OpenGl::RenderBatch::Create(renderComponent.primitive);
}

bool OpenGl::RenderBatcher::IsRegistryEmptyFromBatchedRenderComponents(
    entt::registry& registry) {
    auto renderView = registry.view<SHV::OpenGl::RenderComponent>();
    return renderView.size() == 0;
}