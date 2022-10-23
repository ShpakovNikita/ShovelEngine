#include "Engine/Render/OpenGl/Model/RenderMaterial.hpp"

#include "Engine/Render/OpenGl/Model/GPUTexture.hpp"
#include "Engine/Render/OpenGl/ShaderProgram.hpp"

#include "Engine/Common/Assert.hpp"

using namespace SHV;

OpenGl::RenderMaterial::RenderMaterial(std::shared_ptr<ShaderProgram> aProgram)
    : program(aProgram) {}

OpenGl::RenderMaterial::~RenderMaterial() = default;

void OpenGl::RenderMaterial::Bind() {
    // OpenGl does not support more than 8 textures
    AssertD(textures.size() <= 8);

    uint32_t textureLocation = 0;
    for (auto& [paramName, texture] : textures) {
        program->SetInt(paramName, textureLocation);
        glActiveTexture(GL_TEXTURE0 + textureLocation);
        texture->Bind();
        ++textureLocation;
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
