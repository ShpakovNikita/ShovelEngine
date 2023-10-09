#include "Engine/Render/Metal/Model/GPUTexture.hpp"

#include "Engine/Render/Metal/Model/GPUTextureUtils.hpp"

#include "Engine/Render/Metal/LogicalDevice.hpp"
#include "Engine/Render/Metal/Shaders/ShaderDefinitions.h"

#include "Engine/Common/Assert.hpp"
#include "Engine/Common/Exception.hpp"

#include "Engine/Core/Resources/ResourceManager.hpp"

#include "Metal/Metal.hpp"

using namespace SHV;

SHV::Metal::GPUTexture::GPUTexture(std::weak_ptr<Texture> aTexture,
                                   LogicalDevice& aLogicalDevice,
                                   MTL::BlitCommandEncoder& blitCommandEncoder)
    : texture(aTexture), logicalDevice(aLogicalDevice) {
    if (auto sharedTexture = texture.lock()) {
        CreateMetalTexture(sharedTexture, logicalDevice, blitCommandEncoder);
    } else {
        throw Exception(
            "Cannot create Metal texture! Weak_ptr points to expired "
            "texture!");
    }
}

SHV::Metal::GPUTexture::~GPUTexture() { metalTexture->release(); }

void SHV::Metal::GPUTexture::Bind(MTL::RenderCommandEncoder& encoder,
                                  int location) {
    encoder.setFragmentTexture(metalTexture, location);
    encoder.setFragmentSamplerState(metalSamplerState, location);
}

void SHV::Metal::GPUTexture::CreateMetalTexture(
    const std::shared_ptr<Texture>& texture,
    LogicalDevice& logicalDevice,
    MTL::BlitCommandEncoder& blitCommandEncoder) {
    AssertD(texture != nullptr &&
            texture->GetData() != nullptr);

    MTL::TextureDescriptor* textureDescr = CreateTextureDescriptor(texture);
    textureDescr->setStorageMode(MTL::StorageMode::StorageModeShared);
    textureDescr->setUsage(MTL::TextureUsageShaderRead);

    metalTexture = logicalDevice.GetDevice().newTexture(textureDescr);
    textureDescr->release();

    CopyDataTextureData(texture, metalTexture);

    auto textureSampler = texture->GetTextureSampler();
    auto samplerDescriptor = MTL::SamplerDescriptor::alloc()->init();

    samplerDescriptor->setMinFilter(
        GPUTextureUtils::GetMTLMinMagFilter(textureSampler.minFilter));
    samplerDescriptor->setMagFilter(
        GPUTextureUtils::GetMTLMinMagFilter(textureSampler.magFilter));

    samplerDescriptor->setSAddressMode(
        GPUTextureUtils::GetMTLSamplerAddressMode(textureSampler.addressModeU));
    samplerDescriptor->setTAddressMode(
        GPUTextureUtils::GetMTLSamplerAddressMode(textureSampler.addressModeV));

    if (texture->GetMipmapUsage() != eMipmapsUsage::kNone) {
        samplerDescriptor->setMipFilter(
            GPUTextureUtils::GetMTLMipFilter(textureSampler.mipMinFilter));
    }

    metalSamplerState =
        logicalDevice.GetDevice().newSamplerState(samplerDescriptor);
    samplerDescriptor->release();

    if (texture->GetMipmapUsage() == eMipmapsUsage::kGenerate) {
        blitCommandEncoder.generateMipmaps(metalTexture);
    } else if (texture->GetMipmapUsage() ==
               eMipmapsUsage::kLoadFromData) {
        throw Exception(
            "Mipmaps LoadFromData usage not supported yet in Metal "
            "renderer!");
    }
}

MTL::TextureDescriptor* SHV::Metal::GPUTexture::CreateTextureDescriptor(const std::shared_ptr<Texture>& texture) {
    MTL::TextureDescriptor* textureDescr = CreateBaseTextureDescriptor(texture);

    if (texture->GetMipmapUsage() != eMipmapsUsage::kNone) {
        int heightLevels = ceil(log2(texture->GetHeight()));
        int widthLevels = ceil(log2(texture->GetWidth()));
        int mipCount =
            (heightLevels > widthLevels) ? heightLevels : widthLevels;

        textureDescr->setMipmapLevelCount(mipCount);
    }

    return textureDescr;
}

MTL::TextureDescriptor* SHV::Metal::GPUTexture::CreateBaseTextureDescriptor(const std::shared_ptr<Texture>& texture) {
    MTL::PixelFormat textureFormat =
        GPUTextureUtils::GetMTLTextureFormat(texture->GetTextureFormat());

    switch (texture->GetTextureType()) {
        case eTextureType::kTexture2D:
            return MTL::TextureDescriptor::texture2DDescriptor(
                textureFormat, texture->GetWidth(),
                texture->GetHeight(),
                texture->GetMipmapUsage() != eMipmapsUsage::kNone);
        case eTextureType::kTextureCube:
            return MTL::TextureDescriptor::textureCubeDescriptor(
                textureFormat, texture->GetWidth(), texture->GetMipmapUsage() != eMipmapsUsage::kNone);
    }
}

void SHV::Metal::GPUTexture::CopyDataTextureData(const std::shared_ptr<Texture>& source, MTL::Texture* target) {
    switch (target->textureType()) {
        case MTL::TextureType::TextureTypeCube: {
            CopyDataTextureCubeData(source, target);
            break;
        }
        default: {
            CopyDataTexture2DData(source, target);
        }
    }
}

void SHV::Metal::GPUTexture::CopyDataTextureCubeData(const std::shared_ptr<Texture>& source, MTL::Texture* target) {
    for (uint8_t side = 0; side < 6; ++side) {
        MTL::Region region = MTL::Region::Make2D(
            0, 0, source->GetWidth(), source->GetHeight());
        uint32_t bytesPerRow = source->GetBytesPerChannel() *
                               source->GetChannelsCount() *
                               source->GetWidth();

        target->replaceRegion(region, 0, side, source->GetData(side),
                                    bytesPerRow, bytesPerRow * source->GetHeight());
    }
}

void SHV::Metal::GPUTexture::CopyDataTexture2DData(const std::shared_ptr<Texture>& source, MTL::Texture* target) {
    MTL::Region region = MTL::Region::Make2D(0, 0, source->GetWidth(),
                                             source->GetHeight());
    uint32_t bytesPerRow = source->GetBytesPerChannel() *
                           source->GetChannelsCount() *
                           source->GetWidth();

    target->replaceRegion(region, 0, source->GetData(),
                                bytesPerRow);
}

std::shared_ptr<Texture> SHV::Metal::GPUTexture::MakeTexture() const {
    return GPUTextureUtils::MakeTexture(*metalTexture);
}
