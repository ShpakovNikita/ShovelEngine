#pragma once

#include "Engine/Core/Resources/ResourceCache.hpp"
#include "Engine/Render/Model/Texture.hpp"

namespace SHV {
class TextureLoader : public ResourceLoader<Texture> {
   public:
    TextureLoader() = default;
    virtual ~TextureLoader() = default;

    virtual std::shared_ptr<Texture> Create(const std::string& resourcePath);
    virtual void Release(std::shared_ptr<Texture> resource);
};
}  // namespace SHV