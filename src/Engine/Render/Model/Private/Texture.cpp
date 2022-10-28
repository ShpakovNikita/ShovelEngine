#include "Engine/Render/Model/Texture.hpp"
#include "Engine/Core/Engine.hpp"
#include "Engine/Core/FileSystem.hpp"
#include "Engine/Common/Exception.hpp"

#include "stb/stb_image.h"

#include <map>

using namespace SHV;

namespace SHV::STexture {
static std::map<int, eTextureFormat> channelsCountToTextureFormat = {
    {1, eTextureFormat::kR8},
    {2, eTextureFormat::kRG8},
    {3, eTextureFormat::kRGB8},
    {4, eTextureFormat::kRGBA8},
};
}

Texture::Texture(const void* aData, uint32_t aWidth, uint32_t aHeight,
                 uint32_t aChannelsCount, eTextureFormat aTextureFormat,
                 TextureSampler aTextureSampler)
    : width(aWidth),
      height(aHeight),
      channelsCount(aChannelsCount),
      textureFormat(aTextureFormat),
      textureSampler(aTextureSampler) {
    data = std::malloc(width * height * channelsCount);
    std::memcpy(data, aData, width * height * channelsCount);
}

Texture::Texture(const std::string& aTexturePath) : texturePath(aTexturePath) {
    int w, h, nrChannels;
    const std::string filePath =
        Engine::Get().GetFileSystem().GetPath(texturePath);
    void* stbData = stbi_load(filePath.c_str(), &w, &h, &nrChannels, 0);

    if (!stbData) {
        throw Exception("Failed to load texture %s", texturePath.c_str());
    }

    width = w;
    height = h;
    channelsCount = nrChannels;

    auto textureFormatIt =
        STexture::channelsCountToTextureFormat.find(nrChannels);

    if (textureFormatIt == STexture::channelsCountToTextureFormat.end()) {
        throw Exception("Invalid channels number count! %i", nrChannels);
    }

    textureFormat = textureFormatIt->second;

    data = std::malloc(width * height * channelsCount);
    std::memcpy(data, stbData, width * height * channelsCount);
    stbi_image_free(stbData);
}

Texture::~Texture() {
    std::free(data);
    data = nullptr;
}

uint32_t Texture::GetWidth() const { return width; }

uint32_t Texture::GetHeight() const { return height; }

const std::string& Texture::GetTexturePath() const { return texturePath; }

uint32_t Texture::GetChannelsCount() const { return channelsCount; }

eMipmapsUsage Texture::GetMipmapUsage() const { return mipmapsUsage; }

eTextureFormat Texture::GetTextureFormat() const { return textureFormat; }

const TextureSampler& Texture::GetTextureSampler() const {
    return textureSampler;
}

const void* Texture::GetData() const { return data; }