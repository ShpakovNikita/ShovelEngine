#include "Engine/Render/Model/Texture.hpp"
#include "Engine/Core/Engine.hpp"
#include "Engine/Core/FileSystem.hpp"
#include "Engine/Common/Exception.hpp"

#include "stb/stb_image.h"

using namespace SHV;

Texture::Texture(const std::string& aTexturePath) : texturePath(aTexturePath) {
    int w, h, nrChannels;
    const std::string filePath =
        Engine::Get().GetFileSystem().GetPath(texturePath);
    data = stbi_load(filePath.c_str(), &w, &h, &nrChannels, 0);

    if (!data) {
        throw Exception("Failed to load texture %s", texturePath.c_str());
    }

    width = w;
    height = h;
    channelsCount = nrChannels;
}

Texture::~Texture() {
    stbi_image_free(data);
    data = nullptr;
}

uint32_t Texture::GetWidth() const { return width; }

uint32_t Texture::GetHeight() const { return height; }

const std::string& Texture::GetTexturePath() const { return texturePath; }

uint32_t Texture::GetChannelsCount() const { return channelsCount; }

eMipmapsUsage Texture::GetMipmapUsage() const { return mipmapsUsage; }

const void* Texture::GetData() const { return data; }