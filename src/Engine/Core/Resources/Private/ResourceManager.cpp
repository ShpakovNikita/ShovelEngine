#include "Engine/Core/Resources/ResourceManager.hpp"
#include "Engine/Render/Model/TextureLoader.hpp"

using namespace SHV;

ResourceManager::ResourceManager()
    : texturesCache(std::make_unique<ResourceCache<Texture>>(
          std::make_unique<TextureLoader>())){};

ResourceManager::~ResourceManager() = default;

const std::shared_ptr<Texture> ResourceManager::Get(
    const std::string& resourcePath) {
    return texturesCache->Get(resourcePath);
}

void ResourceManager::Invalidate() { texturesCache->InvalidateCache(); }