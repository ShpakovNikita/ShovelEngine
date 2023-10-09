#pragma once

#include "Engine/Render/Model/Texture.hpp"

#include "Metal/Metal.hpp"

#include <memory>

namespace SHV {
namespace Metal {
struct TextureInfo {
    uint32_t channelsCount;
    uint32_t bytesPerChannel;
};

namespace GPUTextureUtils {
    MTL::PixelFormat GetMTLTextureFormat(eTextureFormat textureFormat);
    MTL::SamplerMinMagFilter GetMTLMinMagFilter(TextureSampler::eFilter filter);
    MTL::SamplerMipFilter GetMTLMipFilter(TextureSampler::eFilter filter);
    MTL::SamplerAddressMode GetMTLSamplerAddressMode(TextureSampler::eAddressMode mode);
    TextureInfo GetTextureInfo(const MTL::PixelFormat& format);
    std::shared_ptr<Texture> MakeTexture(MTL::Texture& texture);
};
}  // namespace Metal
}  // namespace SHV