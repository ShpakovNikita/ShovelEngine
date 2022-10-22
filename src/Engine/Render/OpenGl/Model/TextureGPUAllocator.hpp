#pragma once

#include "Engine/Render/OpenGl/Model/GPUTexture.hpp"
#include "Engine/Core/Resources/ResourceLoader.hpp"

namespace SHV {
namespace OpenGl {
class TextureGPUAllocator : public ResourceLoader<GPUTexture> {
   public:
    TextureGPUAllocator();
    virtual ~TextureGPUAllocator();

    virtual std::shared_ptr<GPUTexture> Create(const std::string& resourcePath);
    virtual void Release(std::shared_ptr<GPUTexture> resource);
};
}  // namespace OpenGl
}  // namespace SHV