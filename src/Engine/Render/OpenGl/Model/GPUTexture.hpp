#pragma once

#include <memory>

#include "GL/glew.h"

#include "Engine/Render/Model/Texture.hpp"

namespace SHV {
namespace OpenGl {
class GPUTexture {
   public:
    GPUTexture(std::shared_ptr<Texture> texture);
    ~GPUTexture();

    void Bind();

   private:
    std::shared_ptr<Texture> texture = nullptr;
    GLuint textureHandle = 0;
};
}  // namespace OpenGl
}  // namespace SHV