#include "Engine/Render/OpenGl/ECS/RenderBatcher.hpp"

#include "Engine/Render/OpenGl/ECS/Components/RenderComponent.hpp"
#include "Engine/Render/OpenGl/Model/GPUTexture.hpp"
#include "Engine/Render/OpenGl/ShaderProgram.hpp"
#include "Engine/Render/OpenGl/ShaderCache.hpp"

#include "Engine/Core/Resources/ResourceCache.hpp"

using namespace SHV;

OpenGl::RenderBatcher::RenderBatcher()
    : gpuTexturesCache(std::make_unique<ResourceCache<GPUTexture>>()),
      shaderCache(std::make_unique<ShaderCache>()) {}

void OpenGl::RenderBatcher::AddRenderBatch(
    entt::registry& registry, entt::entity& entity,
    const SHV::RenderComponent& renderComponent) {
    auto& openGlRenderComponent =
        registry.emplace<SHV::OpenGl::RenderComponent>(entity);
    openGlRenderComponent.renderBatch =
        OpenGl::RenderBatch::Create(renderComponent.primitive);

    openGlRenderComponent.renderMaterial = std::make_shared<RenderMaterial>(
        shaderCache->Get(renderComponent.material.materialShader));

    for (auto& [param, texture] : renderComponent.material.textures) {
        auto gpuTexture = gpuTexturesCache->Get(texture->GetTexturePath());
        openGlRenderComponent.renderMaterial->SetTexture(param, gpuTexture);
    }
}

bool OpenGl::RenderBatcher::IsRegistryEmptyFromBatchedRenderComponents(
    entt::registry& registry) {
    auto renderView = registry.view<SHV::OpenGl::RenderComponent>();
    return renderView.size() == 0;
}