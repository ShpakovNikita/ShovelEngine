#pragma once

#include "ResourceCache.hpp"
#include "Engine/Render/Model/Texture.hpp"

namespace SHV {

class ResourceManager {
   public:
    ResourceManager();
    ~ResourceManager();

    const std::shared_ptr<Texture> Get(const std::string& resourcePath);

    void Invalidate();

   private:
    std::unique_ptr<ResourceCache<Texture>> texturesCache;
};
}  // namespace SHV