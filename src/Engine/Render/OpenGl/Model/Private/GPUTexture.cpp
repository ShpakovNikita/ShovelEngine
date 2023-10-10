#include "Engine/Render/OpenGl/Model/GPUTexture.hpp"
#include "Engine/Render/OpenGl/Model/GPUTextureUtils.hpp"

#include "Engine/Common/Assert.hpp"
#include "Engine/Common/Exception.hpp"

#include "Engine/Core/Engine.hpp"
#include "Engine/Core/Resources/ResourceManager.hpp"

using namespace SHV;

namespace SHV::OpenGl::SGPUTexture {
void GenerateGlTexture(const std::shared_ptr<Texture>& texture) {
    const GlTextureFormat glTextureFormat =
        GPUTextureUtils::GetGlTextureFormat(texture->GetTextureFormat());
    GLuint textureTarget = GPUTextureUtils::GetGlTextureTarget(texture->GetTextureType());
    switch (texture->GetTextureType()) {
        case eTextureType::kTexture2D:
            glTexImage2D(textureTarget, 0, glTextureFormat.internalFormat,
                         texture->GetWidth(), texture->GetHeight(), 0,
                         glTextureFormat.format, glTextureFormat.type,
                         texture->GetData());
            return;
        case eTextureType::kTextureCube:
            for (unsigned int i = 0; i < 6; ++i)
            {
                AssertD(texture->GetHeight() == texture->GetWidth());
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glTextureFormat.internalFormat,
                             texture->GetWidth(), texture->GetHeight(), 0, glTextureFormat.format, glTextureFormat.type, texture->GetData(i));
            }
            return;
    }
}
}  // namespace SHV::OpenGl::GPUTextureUtils

OpenGl::GPUTexture::GPUTexture(std::weak_ptr<Texture> aTexture)
    : texture(aTexture) {
    if (auto sharedTexture = texture.lock()) {
        AssertD(sharedTexture != nullptr &&
                sharedTexture->GetData() != nullptr);
        textureTarget = GPUTextureUtils::GetGlTextureTarget(sharedTexture->GetTextureType());

        glGenTextures(1, &textureHandle);
        glBindTexture(textureTarget, textureHandle);

        GPUTextureUtils::SetupTextureSampler(*sharedTexture);

        SGPUTexture::GenerateGlTexture(sharedTexture);

        if (sharedTexture->GetMipmapUsage() == eMipmapsUsage::kGenerate) {
            glGenerateMipmap(textureTarget);
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
    glBindTexture(textureTarget, textureHandle);
}

void OpenGl::GPUTexture::Unbind() { glBindTexture(textureTarget, 0); }
