#include "Engine/Render/Metal/Model/GPUTextureUtils.hpp"

#include "Engine/Core/Resources/ResourceManager.hpp"

#include "Engine/Common/Exception.hpp"

#include "Metal/Metal.hpp"

using namespace SHV;

MTL::PixelFormat Metal::GPUTextureUtils::GetMTLTextureFormat(eTextureFormat textureFormat) {
    switch (textureFormat) {
        case eTextureFormat::kRGBA8:
            // TODO: should be BGRA8 when proper swizzling is done
            return MTL::PixelFormat::PixelFormatRGBA8Unorm;
        case eTextureFormat::kRG8:
            return MTL::PixelFormat::PixelFormatRG8Unorm;
        case eTextureFormat::kR8:
            return MTL::PixelFormat::PixelFormatR8Unorm;
        case eTextureFormat::kRGBA32F:
            return MTL::PixelFormat::PixelFormatRGBA32Float;
    }
}

std::array<uint8_t, 4> Metal::GPUTextureUtils::GetMTLTextureFormatSwizzling(eTextureFormat textureFormat) {
    switch (textureFormat) {
        case eTextureFormat::kRGBA8:
            return {2, 1, 0, 3};
        case eTextureFormat::kRG8:
            return {0, 1, 0, 0};
        case eTextureFormat::kR8:
            return {0, 0, 0, 0};
        case eTextureFormat::kRGBA32F:
            return {0, 1, 2, 3};
    }
}

MTL::SamplerMinMagFilter Metal::GPUTextureUtils::GetMTLMinMagFilter(TextureSampler::eFilter filter) {
    switch (filter) {
        case TextureSampler::eFilter::kLinear:
            return MTL::SamplerMinMagFilter::SamplerMinMagFilterLinear;
        case TextureSampler::eFilter::kNearest:
            return MTL::SamplerMinMagFilter::SamplerMinMagFilterNearest;
    }
}

MTL::SamplerMipFilter Metal::GPUTextureUtils::GetMTLMipFilter(TextureSampler::eFilter filter) {
    switch (filter) {
        case TextureSampler::eFilter::kLinear:
            return MTL::SamplerMipFilter::SamplerMipFilterLinear;
        case TextureSampler::eFilter::kNearest:
            return MTL::SamplerMipFilter::SamplerMipFilterNearest;
    }
}

MTL::SamplerAddressMode Metal::GPUTextureUtils::GetMTLSamplerAddressMode(
    TextureSampler::eAddressMode mode) {
    switch (mode) {
        case TextureSampler::eAddressMode::kClampToBorder:
            return MTL::SamplerAddressMode::
                SamplerAddressModeClampToBorderColor;
        case TextureSampler::eAddressMode::kClampToEdge:
            return MTL::SamplerAddressMode::SamplerAddressModeClampToEdge;
        case TextureSampler::eAddressMode::kRepeat:
            return MTL::SamplerAddressMode::SamplerAddressModeRepeat;
    }
}

Metal::TextureInfo Metal::GPUTextureUtils::GetTextureInfo(const MTL::PixelFormat& format) {
    switch (format) {
        case MTL::PixelFormat::PixelFormatRGBA8Unorm:
            return {4, 1};
        case MTL::PixelFormat::PixelFormatBGRA8Unorm:
            return {4, 1};
        case MTL::PixelFormat::PixelFormatRG8Unorm:
            return {2, 1};
        case MTL::PixelFormat::PixelFormatR8Unorm:
            return {1, 1};
        case MTL::PixelFormat::PixelFormatRGBA32Float:
            return {4, sizeof(float)};
        default:
            throw SHV::Exception("Could not get info from type: %s\n. Please, define it in this method.",
                                 format);
    }
}

std::shared_ptr<Texture> Metal::GPUTextureUtils::MakeTexture(MTL::Texture& texture) {
    MTL::Region region = MTL::Region::Make2D(
        0, 0, texture.width(), texture.height());
    const TextureInfo textureInfo = GPUTextureUtils::GetTextureInfo(texture.pixelFormat());
    size_t bytesPerRow = textureInfo.bytesPerChannel *
                           textureInfo.channelsCount *
                           texture.width();
    void* data = (void*)malloc(bytesPerRow * texture.height());
    texture.getBytes(data, bytesPerRow, region, 0);

    auto resultTexture = std::make_shared<Texture>(
        data, texture.width(), texture.height(), textureInfo.channelsCount,
        textureInfo.bytesPerChannel /* TODO: make sampler */);

    std::free(data);

    return resultTexture;
}
