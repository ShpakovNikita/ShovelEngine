#pragma once

#include <string>
#include <vector>

namespace SHV {
enum class eMipmapsUsage { kNone, kGenerate, kLoadFromData };
// TODO: ASTC support

enum class eTextureFormat { kRGBA8, kRG8, kR8, kRGBA32F };

enum class eTextureType { kTexture2D, kTextureCube };

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
            uint32_t channelsCount, uint32_t bytesPerChannel,
            TextureSampler textureSampler = {});
    Texture(const std::vector<std::shared_ptr<Texture>>& cubeSides);
    ~Texture();

    uint32_t GetWidth() const;
    uint32_t GetHeight() const;
    const std::string& GetTexturePath() const;
    uint32_t GetChannelsCount() const;
    uint32_t GetBytesPerChannel() const;

    eMipmapsUsage GetMipmapUsage() const;
    eTextureFormat GetTextureFormat() const;

    const TextureSampler& GetTextureSampler() const;

    uint64_t GetBytesSize() const;

    const void* GetData() const;
    const void* GetData(uint8_t slice) const;
    
   private:
    void ClearData();

    eTextureType textureType = eTextureType::kTexture2D;

    std::string texturePath = {};
    uint32_t width = 0, height = 0;
    uint32_t channelsCount = 0;
    uint32_t bytesPerChannel = 1;

    eMipmapsUsage mipmapsUsage = eMipmapsUsage::kGenerate;
    eTextureFormat textureFormat = eTextureFormat::kRGBA8;

    TextureSampler textureSampler;

    // For simplicity, it pretty much duplicates data in GPU texture, for gaming
    // performance this data needs to be cleaned up on scene load
    std::vector<void*> data = {};
};
}  // namespace SHV
