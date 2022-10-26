#include "Engine/Render/OpenGl/Model/GPUTexture.hpp"

#include "Engine/Common/Assert.hpp"
#include "Engine/Common/Exception.hpp"

#include "Engine/Core/Engine.hpp"
#include "Engine/Core/Resources/ResourceManager.hpp"

using namespace SHV;

OpenGl::GPUTexture::GPUTexture(const std::string& texturePath) {
    texture = Engine::Get().GetResourceManager().Get<Texture>(texturePath);

    if (auto sharedTexture = texture.lock()) {
        AssertD(sharedTexture != nullptr &&
                sharedTexture->GetData() != nullptr);

        glGenTextures(1, &textureHandle);
        glBindTexture(GL_TEXTURE_2D, textureHandle);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        if (sharedTexture->GetMipmapUsage() != eMipmapsUsage::kNone) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                            GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                            GL_LINEAR_MIPMAP_LINEAR);
        } else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sharedTexture->GetWidth(),
                     sharedTexture->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
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