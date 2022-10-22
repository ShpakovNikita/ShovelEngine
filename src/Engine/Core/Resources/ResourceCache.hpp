#pragma once

#include <memory>
#include <map>

#include "Engine/Common/Assert.hpp"

namespace SHV {
template <typename ResourceType, typename ResourceLoader>
class ResourceCache final {
   public:
    ResourceCache(std::unique_ptr<ResourceLoader> resourceLoader,
                  size_t cacheSize = 1 * 1000 * 1000 * 1000 /* 1 GB */);
    ~ResourceCache();

    const std::shared_ptr<ResourceType> Get(const std::string& resourcePath);
    void InvalidateCache();

   private:
    std::map<std::string, std::shared_ptr<ResourceType>> cache;
    std::unique_ptr<ResourceLoader> resourceLoader;
    size_t cacheSize;
};
}  // namespace SHV

template <typename ResourceType, typename ResourceLoader>
SHV::ResourceCache<ResourceType, ResourceLoader>::ResourceCache(
    std::unique_ptr<ResourceLoader> aResourceLoader, size_t aCacheSize)
    : resourceLoader(std::move(aResourceLoader)), cacheSize(aCacheSize){};

template <typename ResourceType, typename ResourceLoader>
SHV::ResourceCache<ResourceType,
                   ResourceLoader>::ResourceCache::~ResourceCache() {
    for (auto& [first, second] : cache) {
        AssertD(second.use_count() == 1);

        resourceLoader->Release(second);
    }
}

template <typename ResourceType, typename ResourceLoader>
const std::shared_ptr<ResourceType>
SHV::ResourceCache<ResourceType, ResourceLoader>::Get(
    const std::string& resourcePath) {
    auto it = cache.find(resourcePath);
    if (it != cache.end()) {
        return it->second;
    }

    auto resource = resourceLoader->Create(resourcePath);
    cache[resourcePath] = resource;
    return resource;
}

template <typename ResourceType, typename ResourceLoader>
void SHV::ResourceCache<ResourceType, ResourceLoader>::InvalidateCache() {
    // TODO: implement
}