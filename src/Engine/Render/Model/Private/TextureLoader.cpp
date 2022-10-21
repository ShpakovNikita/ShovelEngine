#include "Engine/Render/Model/TextureLoader.hpp"
#include "Engine/Core/Engine.hpp"
#include "Engine/Core/FileSystem.hpp"
#include "Engine/Common/Exception.hpp"

#include "stb/stb_image.h"

using namespace SHV;

std::shared_ptr<Texture> TextureLoader::Create(
    const std::string& resourcePath) {
    int width, height, nrChannels;
    const std::string filePath =
        Engine::Get().GetFileSystem().GetPath(resourcePath);
    unsigned char* data =
        stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

    if (!data) {
        throw Exception("Failed to load texture %s", resourcePath.c_str());
    }

    auto texture = std::make_shared<Texture>();
    texture->texturePath = resourcePath;
    texture->width = width;
    texture->height = height;
    texture->data = data;

    return texture;
}

void TextureLoader::Release(std::shared_ptr<Texture> resource) {
    stbi_image_free(resource->data);
    resource->data = nullptr;
}