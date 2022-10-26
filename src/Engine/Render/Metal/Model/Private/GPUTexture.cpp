#include "Engine/Render/Metal/Model/GPUTexture.hpp"

#include "Engine/Render/Metal/LogicalDevice.hpp"
#include "Engine/Render/Metal/Shaders/ShaderDefinitions.h"

#include "Engine/Common/Assert.hpp"
#include "Engine/Common/Exception.hpp"

#include "Engine/Core/Engine.hpp"
#include "Engine/Core/Resources/ResourceManager.hpp"

#include "Metal/Metal.hpp"

using namespace SHV;

namespace SHV::Metal::SGPUTexture {
void GetMappedDataRGBA8(u_char* dstImageData, Texture& texture) {
    const u_char* srcImageData = static_cast<const u_char*>(texture.GetData());

    // For every row of the image
    for (uint32_t y = 0; y < texture.GetHeight(); ++y) {
        uint32_t srcRow = y;

        // For every column of the current row
        for (uint32_t x = 0; x < texture.GetWidth(); ++x) {
            uint32_t srcColumn = x;

            // Calculate the index for the first byte of the pixel you're
            // converting in both the source and destination images
            uint32_t srcPixelIndex =
                texture.GetChannelsCount() /* srcBytesPerPixel*/ *
                (srcRow * texture.GetWidth() + srcColumn);
            uint32_t dstPixelIndex = 4 * (y * texture.GetWidth() + x);
            
            // Set the alpha channel of the destination pixel to 255
            dstImageData[dstPixelIndex + 0] = srcImageData[srcPixelIndex + 0];
            dstImageData[dstPixelIndex + 1] = srcImageData[srcPixelIndex + 1];
            dstImageData[dstPixelIndex + 2] = srcImageData[srcPixelIndex + 2];

            dstImageData[dstPixelIndex + 3] = 255;

            if (texture.GetChannelsCount() == 4) {
                dstImageData[dstPixelIndex + 3] =
                    srcImageData[srcPixelIndex + 3];
            } else {
                dstImageData[dstPixelIndex + 3] = 255;
            }
        }
    }
}

MTL::PixelFormat GetMTLTextureFormat(eTextureFormat textureFormat) {
    switch (textureFormat) {
        case eTextureFormat::kRGBA8:
            return MTL::PixelFormat::PixelFormatRGBA8Unorm;
        case eTextureFormat::kRGB8:
            return MTL::PixelFormat::PixelFormatRGBA8Unorm;
        case eTextureFormat::kRG8:
            return MTL::PixelFormat::PixelFormatRG8Unorm;
        case eTextureFormat::kR8:
            return MTL::PixelFormat::PixelFormatR8Unorm;
    }
}
}  // namespace SHV::Metal::SGPUTexture

SHV::Metal::GPUTexture::GPUTexture(const std::string& texturePath,
                                   LogicalDevice& aLogicalDevice,
                                   MTL::BlitCommandEncoder& blitCommandEncoder)
    : logicalDevice(aLogicalDevice) {
    texture = Engine::Get().GetResourceManager().Get<Texture>(texturePath);

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
        MTL::Region region = MTL::Region::Make2D(
            0, 0, sharedTexture->GetWidth(), sharedTexture->GetHeight());
        uint32_t bytesPerRow = 4 * sharedTexture->GetWidth();

        if (sharedTexture->GetTextureFormat() == eTextureFormat::kRGB8) {
            u_char* dstImageData = static_cast<u_char*>(std::malloc(
                sharedTexture->GetWidth() * sharedTexture->GetHeight() * 4));
            SHV::Metal::SGPUTexture::GetMappedDataRGBA8(dstImageData,
                                                        *sharedTexture);
            metalTexture->replaceRegion(region, 0, dstImageData, bytesPerRow);
            std::free(dstImageData);
        } else {
            metalTexture->replaceRegion(region, 0, sharedTexture->GetData(),
                                        bytesPerRow);
        }

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
}
