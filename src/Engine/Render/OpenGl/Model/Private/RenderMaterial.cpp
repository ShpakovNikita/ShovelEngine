#include "Engine/Render/OpenGl/Model/RenderMaterial.hpp"

#include "Engine/Render/OpenGl/Model/GPUTexture.hpp"
#include "Engine/Render/OpenGl/ShaderProgram.hpp"

using namespace SHV;

OpenGl::RenderMaterial::RenderMaterial(std::shared_ptr<ShaderProgram> aProgram)
    : program(aProgram) {}

OpenGl::RenderMaterial::~RenderMaterial() = default;

void OpenGl::RenderMaterial::Bind() {
    for (auto& [first, second] : textures) {
        second->Bind();
    }
}

void OpenGl::RenderMaterial::Unbind() {
    for (auto& [first, second] : textures) {
        second->Unbind();
    }
}

OpenGl::ShaderProgram& OpenGl::RenderMaterial::GetShaderProgram() const {
    return *program;
}

void OpenGl::RenderMaterial::SetTexture(const std::string& paramName,
                                        std::shared_ptr<GPUTexture> texture) {
    textures[paramName] = texture;
}
