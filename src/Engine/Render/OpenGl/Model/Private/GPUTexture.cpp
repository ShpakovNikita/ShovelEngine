#include "Engine/Render/OpenGl/Model/GPUTexture.hpp"

#include "Engine/Common/Assert.hpp"
#include "Engine/Common/Exception.hpp"

#include "Engine/Core/Engine.hpp"
#include "Engine/Core/Resources/ResourceManager.hpp"

using namespace SHV;

namespace SHV::OpenGl::SGPUTexture {
struct GlTextureFormat {
    GLuint internalFormat;  // Format in which we want OpenGl to interpret data
    GLenum format;          // Format to pass data
    GLenum type;            // Data type of passed data
};

GlTextureFormat GetGlTextureFormat(eTextureFormat textureFormat) {
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

GLuint GetGlTextureAddressMode(TextureSampler::eAddressMode mode) {
    switch (mode) {
        case TextureSampler::eAddressMode::kClampToBorder:
            return GL_CLAMP_TO_BORDER;
        case TextureSampler::eAddressMode::kClampToEdge:
            return GL_CLAMP_TO_EDGE;
        case TextureSampler::eAddressMode::kRepeat:
            return GL_REPEAT;
    }
}

GLuint GetGlTextureFilter(TextureSampler::eFilter filter,
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
}  // namespace SHV::OpenGl::SGPUTexture

OpenGl::GPUTexture::GPUTexture(std::weak_ptr<Texture> aTexture)
    : texture(aTexture) {
    if (auto sharedTexture = texture.lock()) {
        AssertD(sharedTexture != nullptr &&
                sharedTexture->GetData() != nullptr);

        glGenTextures(1, &textureHandle);
        glBindTexture(GL_TEXTURE_2D, textureHandle);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                        SGPUTexture::GetGlTextureAddressMode(
                            sharedTexture->GetTextureSampler().addressModeU));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                        SGPUTexture::GetGlTextureAddressMode(
                            sharedTexture->GetTextureSampler().addressModeV));

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        SGPUTexture::GetGlTextureFilter(
                            sharedTexture->GetTextureSampler().minFilter,
                            sharedTexture->GetTextureSampler().mipMinFilter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        SGPUTexture::GetGlTextureFilter(
                            sharedTexture->GetTextureSampler().magFilter,
                            sharedTexture->GetTextureSampler().mipMagFilter));

        const SGPUTexture::GlTextureFormat glTextureFormat =
            SGPUTexture::GetGlTextureFormat(sharedTexture->GetTextureFormat());

        glTexImage2D(GL_TEXTURE_2D, 0, glTextureFormat.internalFormat,
                     sharedTexture->GetWidth(), sharedTexture->GetHeight(), 0,
                     glTextureFormat.format, glTextureFormat.type,
                     sharedTexture->GetData());

        if (sharedTexture->GetMipmapUsage() == eMipmapsUsage::kGenerate) {
            glGenerateMipmap(GL_TEXTURE_2D);
        } else if (sharedTexture->GetMipmapUsage() ==
                   eMipmapsUsage::kLoadFromData) {
            throw Exception(
                "Mipmaps LoadFromData usage not supported yet in OpenGl "
                "renderer!");
        }
    } else {
        throw Exception(
            "Cannot create OpenGl texture! Weak_ptr points to expired "
            "texture!");
    }
}

OpenGl::GPUTexture::~GPUTexture() { glDeleteTextures(1, &textureHandle); }

void OpenGl::GPUTexture::Bind() {
    AssertD(textureHandle != 0);
    glBindTexture(GL_TEXTURE_2D, textureHandle);
}

void OpenGl::GPUTexture::Unbind() { glBindTexture(GL_TEXTURE_2D, 0); }