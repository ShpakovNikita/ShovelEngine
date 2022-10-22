#pragma once

#include <map>

#include "Engine/Render/Model/Material.hpp"

namespace SHV {
namespace OpenGl {
class ShaderProgram;

class ShaderCache {
   public:
    ShaderCache();
    ~ShaderCache();

    const std::shared_ptr<ShaderProgram> Get(eShader shader);

   private:
    std::shared_ptr<ShaderProgram> CreateShaderProgram(eShader shader);

    std::map<eShader, std::shared_ptr<ShaderProgram>> cache;
};
}  // namespace OpenGl
}  // namespace SHV