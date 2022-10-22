#include "Engine/Render/Metal/Model/TextureGPUAllocator.hpp"
#include "Engine/Render/Metal/Model/GPUTexture.hpp"
#include "Engine/Core/Engine.hpp"
#include "Engine/Core/Resources/ResourceManager.hpp"
#include "Engine/Render/Model/Texture.hpp"

using namespace SHV;

Metal::TextureGPUAllocator::TextureGPUAllocator() = default;
Metal::TextureGPUAllocator::~TextureGPUAllocator() = default;

std::shared_ptr<Metal::GPUTexture> Metal::TextureGPUAllocator::Create(
    const std::string& resourcePath) {
    // TODO: Forward texture arg
    const auto texture =
        Engine::Get().GetResourceManager().Get<Texture>(resourcePath);

    auto gpuTexture = std::make_shared<Metal::GPUTexture>();

    return gpuTexture;
}

void Metal::TextureGPUAllocator::Release(
    std::shared_ptr<Metal::GPUTexture> /* resource */) {}
