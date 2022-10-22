#pragma once

#include "ResourceCache.hpp"
#include "ResourceLoader.hpp"
#include "Engine/Render/Model/Texture.hpp"

namespace SHV {

class ResourceManager {
   public:
    ResourceManager();
    ~ResourceManager();

    template <typename ResourceType>
    const std::shared_ptr<ResourceType> Get(const std::string& resourcePath);

    void Invalidate();

   private:
    std::unique_ptr<ResourceCache<Texture>> texturesCache;
};
}  // namespace SHV