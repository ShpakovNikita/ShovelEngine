#include "Engine/Render/OpenGl/ShaderCache.hpp"

#include "Engine/Render/OpenGl/ShaderProgram.hpp"

using namespace SHV;

OpenGl::ShaderCache::ShaderCache() = default;

OpenGl::ShaderCache::~ShaderCache() = default;

const std::shared_ptr<OpenGl::ShaderProgram> OpenGl::ShaderCache::Get(
    eShader shader) {
    auto it = cache.find(shader);
    if (it != cache.end()) {
        return it->second;
    }

    auto shaderProgram = CreateShaderProgram(shader);
    cache[shader] = shaderProgram;
    return shaderProgram;
}

std::shared_ptr<OpenGl::ShaderProgram> OpenGl::ShaderCache::CreateShaderProgram(
    eShader shader) {
    switch (shader) {
        case eShader::kPbrShader:
            return std::make_shared<ShaderProgram>("PbrShader");
        case eShader::kBasicShader:
            return std::make_shared<ShaderProgram>("BasicShader");
    }
}