#pragma once

#include <string>

namespace SHV {
enum class eMipmapsUsage { kNone, kGenerate, kLoadFromData };
// TODO: ASTC support
enum class eTextureFormat { kRGBA8, kRGB8, kRG8, kR8 };

// TODO: Use in renderers
struct TextureSampler {
    enum class eFilter { kLinear, kNearest };
    enum class eAddressMode { kRepeat, kClampToEdge, kClampToBorder };

    eFilter magFilter = eFilter::kLinear;
    eFilter minFilter = eFilter::kLinear;
    
    eFilter mipMagFilter = eFilter::kLinear;
    eFilter mipMinFilter = eFilter::kLinear;

    eAddressMode addressModeU = eAddressMode::kClampToEdge;
    eAddressMode addressModeV = eAddressMode::kClampToEdge;
    eAddressMode addressModeW = eAddressMode::kClampToEdge;
};

class Texture {
   public:
    Texture(const std::string& texturePath);
    Texture(const void* data, uint32_t width, uint32_t height,
            uint32_t channelsCount, eTextureFormat textureFormat,
            TextureSampler textureSampler = {});
    ~Texture();

    uint32_t GetWidth() const;
    uint32_t GetHeight() const;
    const std::string& GetTexturePath() const;
    uint32_t GetChannelsCount() const;

    eMipmapsUsage GetMipmapUsage() const;
    eTextureFormat GetTextureFormat() const;

    const TextureSampler& GetTextureSampler() const;

    const void* GetData() const;

   private:
    std::string texturePath = {};
    uint32_t width = 0, height = 0;
    uint32_t channelsCount = 0;

    eMipmapsUsage mipmapsUsage = eMipmapsUsage::kGenerate;
    eTextureFormat textureFormat = eTextureFormat::kRGBA8;

    TextureSampler textureSampler;

    // TODO: remove from this class?
    void* data = nullptr;
};
}  // namespace SHV