#include "Engine/Render/OpenGl/Model/GPUTextureUtils.hpp"

#include "Engine/Core/Resources/ResourceManager.hpp"

#include "Engine/Common/Exception.hpp"

#include "Metal/Metal.hpp"

using namespace SHV;

OpenGl::GlTextureFormat OpenGl::GPUTextureUtils::GetGlTextureFormat(eTextureFormat textureFormat) {
    switch (textureFormat) {
        case eTextureFormat::kRGBA8:
            return {GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE};
        case eTextureFormat::kRG8:
            return {GL_RG8, GL_RG, GL_UNSIGNED_BYTE};
        case eTextureFormat::kR8:
            return {GL_R8, GL_RED, GL_UNSIGNED_BYTE};
        case eTextureFormat::kRGBA32F:
            return {GL_RGBA32F, GL_RGBA, GL_FLOAT};
    }
}

GLuint OpenGl::GPUTextureUtils::GetGlTextureAddressMode(TextureSampler::eAddressMode mode) {
    switch (mode) {
        case TextureSampler::eAddressMode::kClampToBorder:
            return GL_CLAMP_TO_BORDER;
        case TextureSampler::eAddressMode::kClampToEdge:
            return GL_CLAMP_TO_EDGE;
        case TextureSampler::eAddressMode::kRepeat:
            return GL_REPEAT;
    }
}

GLuint OpenGl::GPUTextureUtils::GetGlTextureFilter(TextureSampler::eFilter filter,
                          TextureSampler::eFilter mipFilter) {
    if (filter == TextureSampler::eFilter::kNearest &&
        mipFilter == TextureSampler::eFilter::kNearest) {
        return GL_NEAREST_MIPMAP_NEAREST;
    } else if (filter == TextureSampler::eFilter::kLinear &&
               mipFilter == TextureSampler::eFilter::kNearest) {
        return GL_LINEAR_MIPMAP_NEAREST;
    } else if (filter == TextureSampler::eFilter::kNearest &&
               mipFilter == TextureSampler::eFilter::kLinear) {
        return GL_NEAREST_MIPMAP_LINEAR;
    } else {
        return GL_LINEAR_MIPMAP_LINEAR;
    }
}

GLuint OpenGl::GPUTextureUtils::GetGlTextureTarget(eTextureType type) {
    switch (type) {
        case eTextureType::kTexture2D:
            return GL_TEXTURE_2D;
        case eTextureType::kTextureCube:
            return GL_TEXTURE_CUBE_MAP;
    }
}

void OpenGl::GPUTextureUtils::SetupTextureSampler(const Texture& texture) {
    GLuint textureTarget = GPUTextureUtils::GetGlTextureTarget(texture.GetTextureType());

    glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S,
                    GPUTextureUtils::GetGlTextureAddressMode(
                        texture.GetTextureSampler().addressModeU));
    glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T,
                    GPUTextureUtils::GetGlTextureAddressMode(
                        texture.GetTextureSampler().addressModeV));
    glTexParameteri(textureTarget, GL_TEXTURE_WRAP_R,
                    GPUTextureUtils::GetGlTextureAddressMode(
                        texture.GetTextureSampler().addressModeW));

    glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER,
                    GPUTextureUtils::GetGlTextureFilter(
                        texture.GetTextureSampler().minFilter,
                        texture.GetTextureSampler().mipMinFilter));
    glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER,
                    GPUTextureUtils::GetGlTextureFilter(
                        texture.GetTextureSampler().magFilter,
                        texture.GetTextureSampler().mipMagFilter));
}

std::shared_ptr<Texture> OpenGl::GPUTextureUtils::MakeTextureFromFrameBuffer(GLuint& fbo, const Texture& prototype) {
    void* data = (void*)malloc(prototype.GetBytesSize());
    GlTextureFormat format = GPUTextureUtils::GetGlTextureFormat(prototype.GetTextureFormat());
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glReadPixels(0, 0, prototype.GetWidth(), prototype.GetHeight(), format.format, format.type, data);
    auto resultTexture = std::make_shared<Texture>(
        data, prototype.GetWidth(), prototype.GetHeight(), prototype.GetChannelsCount(),
        prototype.GetBytesPerChannel() /* TODO: make sampler */);
    std::free(data);
    return resultTexture;
}