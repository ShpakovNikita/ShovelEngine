#pragma once

#include <memory>
#include <map>

#include "Engine/Common/Assert.hpp"

namespace SHV {
template <typename ResourceType>
class ResourceLoader {
   public:
    ResourceLoader() = default;
    virtual ~ResourceLoader() = default;

    virtual std::shared_ptr<ResourceType> Create(
        const std::string& /* resourcePath */) = 0;
    virtual void Release(std::shared_ptr<ResourceType> /* resource */) = 0;
};

template <typename ResourceType>
class ResourceCache final {
   public:
    ResourceCache(std::unique_ptr<ResourceLoader<ResourceType>> resourceLoader,
                  size_t cacheSize = 1 * 1000 * 1000 * 1000 /* 1 GB */);
    ~ResourceCache();

    const std::shared_ptr<ResourceType> Get(const std::string& resourcePath);
    void InvalidateCache();

   private:
    std::map<std::string, std::shared_ptr<ResourceType>> cache;
    std::unique_ptr<ResourceLoader<ResourceType>> resourceLoader;
    size_t cacheSize;
};
}  // namespace SHV

template <typename ResourceType>
SHV::ResourceCache<ResourceType>::ResourceCache(
    std::unique_ptr<ResourceLoader<ResourceType>> aResourceLoader, size_t aCacheSize)
    : resourceLoader(std::move(aResourceLoader)), cacheSize(aCacheSize){};

template <typename ResourceType>
SHV::ResourceCache<ResourceType>::ResourceCache::~ResourceCache() {
    for (auto& [first, second] : cache) {
        AssertD(second.use_count() == 1);

        resourceLoader->Release(second);
    }
}

template <typename ResourceType>
const std::shared_ptr<ResourceType> SHV::ResourceCache<ResourceType>::Get(
    const std::string& resourcePath) {
    auto it = cache.find(resourcePath);
    if (it != cache.end()) {
        return it->second;
    }

    auto resource = resourceLoader->Create(resourcePath);
    cache[resourcePath] = resource;
    return resource;
}

template <typename ResourceType>
void SHV::ResourceCache<ResourceType>::InvalidateCache() {
    // TODO: implement
}