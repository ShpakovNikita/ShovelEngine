#include "Engine/Render/Metal/Model/GPUTexture.hpp"

#include "Engine/Render/Model/Utils/TextureUtils.hpp"
#include "Engine/Render/Metal/LogicalDevice.hpp"
#include "Engine/Render/Metal/Shaders/ShaderDefinitions.h"

#include "Engine/Common/Assert.hpp"
#include "Engine/Common/Exception.hpp"

#include "Engine/Core/Engine.hpp"
#include "Engine/Core/Resources/ResourceManager.hpp"

#include "Metal/Metal.hpp"

using namespace SHV;

namespace SHV::Metal::SGPUTexture {
MTL::PixelFormat GetMTLTextureFormat(eTextureFormat textureFormat) {
    switch (textureFormat) {
        case eTextureFormat::kRGBA8:
            return MTL::PixelFormat::PixelFormatRGBA8Unorm;
        case eTextureFormat::kRG8:
            return MTL::PixelFormat::PixelFormatRG8Unorm;
        case eTextureFormat::kR8:
            return MTL::PixelFormat::PixelFormatR8Unorm;
        case eTextureFormat::kRGBA32F:
            return MTL::PixelFormat::PixelFormatRGBA32Float;
    }
}

MTL::SamplerMinMagFilter GetMTLMinMagFilter(TextureSampler::eFilter filter) {
    switch (filter) {
        case TextureSampler::eFilter::kLinear:
            return MTL::SamplerMinMagFilter::SamplerMinMagFilterLinear;
        case TextureSampler::eFilter::kNearest:
            return MTL::SamplerMinMagFilter::SamplerMinMagFilterNearest;
    }
}

MTL::SamplerMipFilter GetMTLMipFilter(TextureSampler::eFilter filter) {
    switch (filter) {
        case TextureSampler::eFilter::kLinear:
            return MTL::SamplerMipFilter::SamplerMipFilterLinear;
        case TextureSampler::eFilter::kNearest:
            return MTL::SamplerMipFilter::SamplerMipFilterNearest;
    }
}

MTL::SamplerAddressMode GetMTLSamplerAddressMode(
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
}  // namespace SHV::Metal::SGPUTexture

SHV::Metal::GPUTexture::GPUTexture(std::weak_ptr<Texture> aTexture,
                                   LogicalDevice& aLogicalDevice,
                                   MTL::BlitCommandEncoder& blitCommandEncoder)
    : texture(aTexture), logicalDevice(aLogicalDevice) {
    if (auto sharedTexture = texture.lock()) {
        AssertD(sharedTexture != nullptr &&
                sharedTexture->GetData() != nullptr);

        MTL::PixelFormat textureFormat =
            SGPUTexture::GetMTLTextureFormat(sharedTexture->GetTextureFormat());

        auto textureDescr = MTL::TextureDescriptor::texture2DDescriptor(
            textureFormat, sharedTexture->GetWidth(),
            sharedTexture->GetHeight(),
            sharedTexture->GetMipmapUsage() != eMipmapsUsage::kNone);
        textureDescr->setStorageMode(MTL::StorageMode::StorageModeShared);
        textureDescr->setUsage(MTL::TextureUsageShaderRead);

        if (sharedTexture->GetMipmapUsage() != eMipmapsUsage::kNone) {
            int heightLevels = ceil(log2(sharedTexture->GetHeight()));
            int widthLevels = ceil(log2(sharedTexture->GetWidth()));
            int mipCount =
                (heightLevels > widthLevels) ? heightLevels : widthLevels;

            textureDescr->setMipmapLevelCount(mipCount);
        }

        metalTexture = logicalDevice.GetDevice().newTexture(textureDescr);
        textureDescr->release();

        MTL::Region region = MTL::Region::Make2D(
            0, 0, sharedTexture->GetWidth(), sharedTexture->GetHeight());
        uint32_t bytesPerRow = sharedTexture->GetBytesPerChannel() *
                               sharedTexture->GetChannelsCount() *
                               sharedTexture->GetWidth();

        metalTexture->replaceRegion(region, 0, sharedTexture->GetData(),
                                    bytesPerRow);

        auto textureSampler = sharedTexture->GetTextureSampler();
        auto samplerDescriptor = MTL::SamplerDescriptor::alloc()->init();

        samplerDescriptor->setMinFilter(
            SGPUTexture::GetMTLMinMagFilter(textureSampler.minFilter));
        samplerDescriptor->setMagFilter(
            SGPUTexture::GetMTLMinMagFilter(textureSampler.magFilter));

        samplerDescriptor->setSAddressMode(
            SGPUTexture::GetMTLSamplerAddressMode(textureSampler.addressModeU));
        samplerDescriptor->setTAddressMode(
            SGPUTexture::GetMTLSamplerAddressMode(textureSampler.addressModeV));

        if (sharedTexture->GetMipmapUsage() != eMipmapsUsage::kNone) {
            samplerDescriptor->setMipFilter(
                SGPUTexture::GetMTLMipFilter(textureSampler.mipMinFilter));
        }

        metalSamplerState =
            logicalDevice.GetDevice().newSamplerState(samplerDescriptor);
        samplerDescriptor->release();

        if (sharedTexture->GetMipmapUsage() == eMipmapsUsage::kGenerate) {
            blitCommandEncoder.generateMipmaps(metalTexture);
        } else if (sharedTexture->GetMipmapUsage() ==
                   eMipmapsUsage::kLoadFromData) {
            throw Exception(
                "Mipmaps LoadFromData usage not supported yet in Metal "
                "renderer!");
        }
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
