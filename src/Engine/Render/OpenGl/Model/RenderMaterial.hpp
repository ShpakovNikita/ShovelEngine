#pragma once

#include <memory>
#include <map>

namespace SHV {
namespace OpenGl {
class ShaderProgram;
class GPUTexture;

class RenderMaterial {
   public:
    RenderMaterial(std::shared_ptr<ShaderProgram> program);
    ~RenderMaterial();

    void Bind();
    void Unbind();

    void SetTexture(const std::string& paramName,
                    std::shared_ptr<GPUTexture> texture);

    ShaderProgram& GetShaderProgram() const;

   private:
    void Invalidate();

    std::map<std::string, std::shared_ptr<GPUTexture>> textures;
    std::shared_ptr<ShaderProgram> program;

    bool invalidate = true;
};
}  // namespace OpenGl
}  // namespace SHV