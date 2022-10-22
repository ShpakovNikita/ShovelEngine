#pragma once

#include <memory>
#include <string>
#include <map>

#include "Engine/Common/Assert.hpp"

namespace SHV {
template <typename ResourceType>
class ResourceCache final {
   public:
    ResourceCache(size_t cacheSize = 1 * 1000 * 1000 * 1000 /* 1 GB */);
    ~ResourceCache();

    const std::shared_ptr<ResourceType> Get(const std::string& resourcePath);
    void InvalidateCache();

   private:
    std::map<std::string, std::shared_ptr<ResourceType>> cache;
    size_t cacheSize;
};
}  // namespace SHV

template <typename ResourceType>
SHV::ResourceCache<ResourceType>::ResourceCache(size_t aCacheSize)
    : cacheSize(aCacheSize){};

template <typename ResourceType>
SHV::ResourceCache<ResourceType>::ResourceCache::~ResourceCache() {
    for (auto& [first, second] : cache) {
        AssertD(second.use_count() == 1);
    }
}

template <typename ResourceType>
const std::shared_ptr<ResourceType> SHV::ResourceCache<ResourceType>::Get(
    const std::string& resourcePath) {
    auto it = cache.find(resourcePath);
    if (it != cache.end()) {
        return it->second;
    }

    auto resource = std::make_shared<ResourceType>(resourcePath);
    cache[resourcePath] = resource;
    return resource;
}

template <typename ResourceType>
void SHV::ResourceCache<ResourceType>::InvalidateCache() {
    // TODO: implement
}