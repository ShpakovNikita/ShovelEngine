#pragma once

#include <memory>

#include "GL/glew.h"

#include "Engine/Render/Model/Texture.hpp"

namespace SHV {
namespace OpenGl {
class GPUTexture {
   public:
    GPUTexture(std::weak_ptr<Texture> texture);
    ~GPUTexture();

    void Bind();
    void Unbind();

   private:
    std::weak_ptr<Texture> texture;
    GLuint textureHandle = 0;
    GLuint textureTarget = 0;
};
}  // namespace OpenGl
}  // namespace SHV