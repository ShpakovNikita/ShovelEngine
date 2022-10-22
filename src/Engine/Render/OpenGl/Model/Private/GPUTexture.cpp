#include "Engine/Render/OpenGl/Model/GPUTexture.hpp"

#include "Engine/Common/Assert.hpp"

using namespace SHV;

OpenGl::GPUTexture::GPUTexture(std::shared_ptr<Texture> aTexture)
    : texture(aTexture) {
    glGenTextures(1, &textureHandle);
    glBindTexture(GL_TEXTURE_2D, textureHandle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    AssertD(texture != nullptr && texture->data != nullptr);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, texture->data);
}

OpenGl::GPUTexture::~GPUTexture() { glDeleteTextures(1, &textureHandle); }

void OpenGl::GPUTexture::Bind() { glBindTexture(GL_TEXTURE_2D, textureHandle); }