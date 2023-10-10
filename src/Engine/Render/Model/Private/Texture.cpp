#include "Engine/Render/Model/Texture.hpp"
#include "Engine/Core/Engine.hpp"
#include "Engine/Core/FileSystem.hpp"
#include "Engine/Common/Exception.hpp"
#include "Engine/Render/Model/Utils/TextureUtils.hpp"

#include "Engine/Common/Assert.hpp"

#include "stb/stb_image.h"

#include <map>

using namespace SHV;

namespace SHV::STexture {
static std::map<int, eTextureFormat>
    channelsCountToTextureFormatForOneByteFormat = {
        {1, eTextureFormat::kR8},
        {2, eTextureFormat::kRG8},
        {3, eTextureFormat::kRGBA8},
        {4, eTextureFormat::kRGBA8},
};

static std::map<int, eTextureFormat>
    channelsCountToTextureFormatForFloatFormat = {
        {3, eTextureFormat::kRGBA32F},
        {4, eTextureFormat::kRGBA32F},
};

eTextureFormat GetTextureFormat(uint32_t channelsCount,
                                uint32_t bytesPerChannel) {
    if (bytesPerChannel == 1) {
        auto textureFormatIt =
            channelsCountToTextureFormatForOneByteFormat.find(channelsCount);

        if (textureFormatIt ==
            channelsCountToTextureFormatForOneByteFormat.end()) {
            throw Exception(
                "Invalid channels number count for one byte per channel type! "
                "%i",
                channelsCount);
        }

        return textureFormatIt->second;
    } else if (bytesPerChannel == sizeof(float)) {
        auto textureFormatIt =
            channelsCountToTextureFormatForFloatFormat.find(channelsCount);

        if (textureFormatIt ==
            channelsCountToTextureFormatForFloatFormat.end()) {
            throw Exception(
                "Invalid channels number count for float channel type! "
                "%i",
                channelsCount);
        }

        return textureFormatIt->second;
    }

    throw Exception(
        "Invalid channels number count for one byte per channel type! %i",
        channelsCount);
}

void* GetConvertedData(eTextureFormat textureFormat, uint32_t height,
                       uint32_t width, uint32_t& channelsCount,
                       uint32_t bytesPerChannel, const void* src) {
    if (textureFormat == eTextureFormat::kRGBA8 && channelsCount == 3) {
        channelsCount = 4;
        return SHV::TextureUtils::RemapTextureDataOneByte(
            static_cast<const uint8_t*>(src), width, height, 3, 4);
    } else if (textureFormat == eTextureFormat::kRGBA32F &&
               channelsCount == 3) {
        channelsCount = 4;
        return SHV::TextureUtils::RemapTextureDataFloat(
            static_cast<const uint8_t*>(src), width, height, 3, 4);
    } else {
        void* data =
            std::malloc(width * height * channelsCount * bytesPerChannel);
        std::memcpy(data, src,
                    width * height * channelsCount * bytesPerChannel);
        return data;
    }
};
}  // namespace SHV::STexture

Texture::Texture(const void* aData, uint32_t aWidth, uint32_t aHeight,
                 uint32_t aChannelsCount, uint32_t aBytesPerChannel,
                 TextureSampler aTextureSampler)
    : width(aWidth),
      height(aHeight),
      channelsCount(aChannelsCount),
      bytesPerChannel(aBytesPerChannel),
      textureSampler(aTextureSampler) {
    textureFormat = STexture::GetTextureFormat(channelsCount, bytesPerChannel);
    if (aData != nullptr) {
        data.resize(1);
        data[0] =
            STexture::GetConvertedData(textureFormat, height, width,
                                       channelsCount, bytesPerChannel, aData);
    }
}

Texture::Texture(const std::vector<std::shared_ptr<Texture>>& cubeSides)
    : textureType(eTextureType::kTextureCube) {
    AssertD(cubeSides.size() >= 6)
    Texture* prototype = cubeSides[0].get();
    width = prototype->width;
    height = prototype->height;

    AssertD(width == height)
    channelsCount = prototype->channelsCount;
    bytesPerChannel = prototype->bytesPerChannel;
    textureFormat = prototype->textureFormat;

    data.resize(6);
    for (size_t i = 0; i < data.size(); ++i) {
        Texture* sideTexture = cubeSides[i].get();
        AssertD(sideTexture->channelsCount == channelsCount &&
                sideTexture->bytesPerChannel == bytesPerChannel &&
                sideTexture->height == height &&
                sideTexture->width == width)
        data[i] = (void*)malloc(sideTexture->GetBytesSize());
        std::memcpy(data[i], sideTexture->GetData(), sideTexture->GetBytesSize());
    }
}

Texture::Texture(const std::string& aTexturePath) : texturePath(aTexturePath) {
    int w, h, nrChannels;
    const std::string filePath =
        Engine::Get().GetFileSystem().GetPath(texturePath);

    // Note, that while this is true for open gl and metal, with introduction of
    // DirectX this thing needs to be revised
    stbi_set_flip_vertically_on_load(1);

    void* stbData;
    if (stbi_is_hdr(filePath.c_str())) {
        stbData = stbi_loadf(filePath.c_str(), &w, &h, &nrChannels, 0);
        bytesPerChannel = sizeof(float);
    } else {
        stbData = stbi_load(filePath.c_str(), &w, &h, &nrChannels, 0);
    }

    if (!stbData) {
        throw Exception("Failed to load texture %s", texturePath.c_str());
    }

    width = w;
    height = h;
    channelsCount = nrChannels;

    textureFormat = STexture::GetTextureFormat(nrChannels, bytesPerChannel);

    data.resize(1);
    data[0] = STexture::GetConvertedData(textureFormat, height, width,
                                      channelsCount, bytesPerChannel, stbData);
    stbi_image_free(stbData);
    stbi_set_flip_vertically_on_load(0);
}

Texture::~Texture() {
    ClearData();
}

uint32_t Texture::GetWidth() const { return width; }

uint32_t Texture::GetHeight() const { return height; }

const std::string& Texture::GetTexturePath() const { return texturePath; }

uint32_t Texture::GetChannelsCount() const { return channelsCount; }

uint32_t Texture::GetBytesPerChannel() const { return bytesPerChannel; }

eTextureType Texture::GetTextureType() const { return textureType; }

eMipmapsUsage Texture::GetMipmapUsage() const { return mipmapsUsage; }

eTextureFormat Texture::GetTextureFormat() const { return textureFormat; }

const TextureSampler& Texture::GetTextureSampler() const {
    return textureSampler;
}

uint64_t Texture::GetBytesSize() const {
    return channelsCount * bytesPerChannel * width * height;
}

const void* Texture::GetData() const {
    return !data.empty() ? data[0] : nullptr;
}

const void* Texture::GetData(uint8_t slice) const {
    AssertD(textureType == eTextureType::kTextureCube && slice < data.size())
    return data[slice];
}

void Texture::ClearData() {
    for (size_t i = 0; i < data.size(); ++i) {
        std::free(data[i]);
    }
    data.clear();
}
