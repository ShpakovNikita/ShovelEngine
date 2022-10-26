#pragma once

#include <string>

namespace SHV {
enum class eMipmapsUsage { kNone, kGenerate, kLoadFromData };
// TODO: ASTC support
enum class eTextureFormat { kRGBA8, kRGB8, kRG8, kR8 };

class Texture {
   public:
    Texture(const std::string& texturePath);
    ~Texture();

    uint32_t GetWidth() const;
    uint32_t GetHeight() const;
    const std::string& GetTexturePath() const;
    uint32_t GetChannelsCount() const;

    eMipmapsUsage GetMipmapUsage() const;
    eTextureFormat GetTextureFormat() const;

    const void* GetData() const;

   private:
    std::string texturePath;
    uint32_t width = 0, height = 0;
    uint32_t channelsCount = 0;

    eMipmapsUsage mipmapsUsage = eMipmapsUsage::kGenerate;
    eTextureFormat textureFormat = eTextureFormat::kRGBA8;

    // TODO: remove from this class?
    void* data = nullptr;
};
}  // namespace SHV