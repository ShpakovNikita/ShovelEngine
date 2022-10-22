#pragma once

#include <memory>

#include "GL/glew.h"

#include "Engine/Render/Model/Texture.hpp"

namespace SHV {
namespace OpenGl {
class GPUTexture {
   public:
    GPUTexture(const std::string& texturePath);
    ~GPUTexture();

    void Bind();
    void Unbind();

   private:
    std::weak_ptr<Texture> texture;
    GLuint textureHandle = 0;
};
}  // namespace OpenGl
}  // namespace SHV