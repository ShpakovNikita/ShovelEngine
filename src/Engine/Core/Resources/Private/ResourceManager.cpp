#include "Engine/Core/Resources/ResourceManager.hpp"

using namespace SHV;

ResourceManager::ResourceManager()
    : texturesCache(std::make_unique<ResourceCache<Texture>>()){};

ResourceManager::~ResourceManager() = default;

template <>
const std::shared_ptr<Texture> ResourceManager::Get(
    const std::string& resourcePath) {
    return texturesCache->Get(resourcePath);
}

void ResourceManager::Invalidate() { texturesCache->InvalidateCache(); }