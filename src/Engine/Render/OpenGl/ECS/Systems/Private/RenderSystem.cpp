#include "Engine/Render/OpenGl/ECS/Systems/RenderSystem.hpp"

#include "Engine/Common/Assert.hpp"

#include "Engine/ECS/Components/RenderComponent.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/Render/OpenGl/ECS/Components/RenderComponent.hpp"

using namespace SHV;

namespace SHV::OpenGl::SRenderSystem {
bool IsRegistryEmptyFromBatchedRenderComponents(entt::registry& registry) {
    auto renderView = registry.view<SHV::OpenGl::RenderComponent>();
    return renderView.size() == 0;
}
}  // namespace SHV::OpenGl::SRenderSystem

OpenGl::RenderSystem::RenderSystem(entt::registry& registry)
    : System(registry) {}
OpenGl::RenderSystem::~RenderSystem() = default;

void OpenGl::RenderSystem::Process(float /*dt*/) {
    auto renderView =
        registry.view<SHV::RenderComponent, SHV::TransformComponent>();

    for (auto&& [entity, renderComponent, transformComponent] :
         renderView.each()) {
        const auto* openGlRenderComponent =
            registry.try_get<SHV::OpenGl::RenderComponent>(entity);

        if (openGlRenderComponent == nullptr) {
            AddRenderBatch(entity, renderComponent);
        }
    }
}

void OpenGl::RenderSystem::AddRenderBatch(
    entt::entity& entity, const SHV::RenderComponent& renderComponent) {
    auto& openGlRenderComponent =
        registry.emplace<SHV::OpenGl::RenderComponent>(entity);
    openGlRenderComponent.renderBatch =
        OpenGl::RenderBatch::Create(renderComponent.primitive);
}

void OpenGl::RenderSystem::SetUp() {
    AssertD(OpenGl::SRenderSystem::IsRegistryEmptyFromBatchedRenderComponents(
        registry));
}

void OpenGl::RenderSystem::TearDown() {
    auto renderView = registry.view<SHV::OpenGl::RenderComponent>();

    for (auto&& [entity, renderComponent] : renderView.each()) {
        renderComponent.renderBatch.Release();
    }

    registry.clear<SHV::OpenGl::RenderComponent>();
}