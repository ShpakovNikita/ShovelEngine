#pragma once

#include "Engine/Render/Model/Texture.hpp"

#include "GL/glew.h"

namespace SHV {
namespace OpenGl {
struct TextureInfo {
    uint32_t channelsCount;
    uint32_t bytesPerChannel;
};

struct GlTextureFormat {
    GLuint internalFormat;  // Format in which we want OpenGl to interpret data
    GLenum format;          // Format to pass data
    GLenum type;            // Data type of passed data
};

namespace GPUTextureUtils {
GlTextureFormat GetGlTextureFormat(eTextureFormat textureFormat);
GLuint GetGlTextureAddressMode(TextureSampler::eAddressMode mode);
GLuint GetGlTextureFilter(TextureSampler::eFilter filter,
                          TextureSampler::eFilter mipFilter);
GLuint GetGlTextureTarget(eTextureType type);
void SetupTextureSampler(const Texture& texture);
std::shared_ptr<Texture> MakeTextureFromFrameBuffer(GLuint& fbo, const Texture& prototype);
};
}  // namespace OpenGl
}  // namespace SHV