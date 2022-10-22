#include "Engine/Render/OpenGl/Model/TextureGPUAllocator.hpp"
#include "Engine/Render/OpenGl/Model/GPUTexture.hpp"
#include "Engine/Core/Engine.hpp"
#include "Engine/Core/Resources/ResourceManager.hpp"
#include "Engine/Render/Model/Texture.hpp"

#include "GL/glew.h"

using namespace SHV;

OpenGl::TextureGPUAllocator::TextureGPUAllocator() = default;
OpenGl::TextureGPUAllocator::~TextureGPUAllocator() = default;

std::shared_ptr<OpenGl::GPUTexture> OpenGl::TextureGPUAllocator::Create(
    const std::string& resourcePath) {
    // TODO: Forward texture arg
    const auto texture =
        Engine::Get().GetResourceManager().Get<Texture>(resourcePath);

    auto gpuTexture = std::make_shared<OpenGl::GPUTexture>(texture);

    return gpuTexture;
}

void OpenGl::TextureGPUAllocator::Release(
    std::shared_ptr<OpenGl::GPUTexture> /* resource */) {}
