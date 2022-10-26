#include "Engine/Render/OpenGl/Model/RenderMaterial.hpp"

#include "Engine/Render/OpenGl/Model/GPUTexture.hpp"
#include "Engine/Render/OpenGl/ShaderProgram.hpp"

#include "Engine/Common/Assert.hpp"
#include "Engine/Common/Logger.hpp"

using namespace SHV;

OpenGl::RenderMaterial::RenderMaterial(std::shared_ptr<ShaderProgram> aProgram)
    : program(aProgram) {}

OpenGl::RenderMaterial::~RenderMaterial() = default;

void OpenGl::RenderMaterial::Bind() {
    // OpenGl does not support more than 8 textures
    AssertD(textures.size() <= 8);

    if (invalidate) {
        Invalidate();
    }

    uint32_t textureLocation = 0;
    for (auto& [paramName, texture] : textures) {
        bool paramInitialized = program->SetInt(paramName, textureLocation);

        if (paramInitialized) {
            glActiveTexture(GL_TEXTURE0 + textureLocation);
            texture->Bind();
            ++textureLocation;
        }
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
    invalidate = true;
}

void OpenGl::RenderMaterial::Invalidate() {
    // OpenGl does not support more than 8 textures
    AssertD(textures.size() <= 8);

    AssertD(invalidate == true);

    uint32_t textureLocation = 0;
    for (auto& [paramName, texture] : textures) {
        bool paramInitialized = program->SetInt(paramName, textureLocation);

        if (!paramInitialized) {
            LogW(eTag::kOpenGl)
                << "Failed to find texture location for param name "
                << paramName << std::endl;
        }
    }

    invalidate = false;
}