#include "Engine/Render/OpenGl/ShaderCache.hpp"

#include "Engine/Render/OpenGl/ShaderProgram.hpp"

using namespace SHV;

OpenGl::ShaderCache::ShaderCache() = default;

OpenGl::ShaderCache::~ShaderCache() = default;

const std::shared_ptr<OpenGl::ShaderProgram> OpenGl::ShaderCache::Get(
    eMaterialShader shader) {
    auto it = cache.find(shader);
    if (it != cache.end()) {
        return it->second;
    }

    auto shaderProgram = CreateShaderProgram(shader);
    cache[shader] = shaderProgram;
    return shaderProgram;
}

std::shared_ptr<OpenGl::ShaderProgram> OpenGl::ShaderCache::CreateShaderProgram(
    eMaterialShader shader) {
    switch (shader) {
        case eMaterialShader::kPbrShader:
            return std::make_shared<ShaderProgram>("PbrShader");
        case eMaterialShader::kBasicShader:
            return std::make_shared<ShaderProgram>("BasicShader");
        case eMaterialShader::kSkyboxShader:
            return std::make_shared<ShaderProgram>("SkyboxShader");
        case eMaterialShader::kSkyboxEquirectangularShader:
            return std::make_shared<ShaderProgram>("SkyboxShader.vert", "SkyboxEquirectangularShader.frag");
    }
}
