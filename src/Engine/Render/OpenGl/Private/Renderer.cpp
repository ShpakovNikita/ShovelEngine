#include "Engine/Render/OpenGl/Renderer.hpp"

#include <SDL_video.h>

#include <vector>

#include "Engine/Common/Exception.hpp"
#include "Engine/Render/OpenGl/ShaderProgram.hpp"
#include "Engine/Utils/Assert.hpp"

SHV::OpenGl::Renderer::Renderer(SHV::OpenGl::Window& /*window*/)
    : SHV::Renderer(){};

SHV::OpenGl::Renderer::~Renderer() = default;

void SHV::OpenGl::Renderer::SetUp() {
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        throw SHV::Exception("Error initializing GLEW! %s\n",
                             glewGetErrorString(glewError));
    }

    // Use Vsync
    if (SDL_GL_SetSwapInterval(1) < 0) {
        throw SHV::Exception("Unable to set VSync! SDL Error: %s\n",
                             SDL_GetError());
    }

    AssertD(program == nullptr);
    program = std::make_unique<ShaderProgram>("BasicShader");
    program->SetUp();

    throw SHV::Exception("Open GL Renderer not implemented!");
}

void SHV::OpenGl::Renderer::TearDown() {
    AssertD(program != nullptr);
    program->TearDown();
    program = nullptr;

    throw SHV::Exception("Open GL Renderer not implemented!");
}

void SHV::OpenGl::Renderer::Draw() {
    throw SHV::Exception("Open GL Renderer not implemented!");
}
