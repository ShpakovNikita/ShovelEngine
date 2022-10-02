#include "Engine/Render/OpenGl/Window.hpp"

#include <SDL_video.h>

#include "Engine/Common/Assert.hpp"
#include "Engine/Common/Exception.hpp"

SHV::OpenGl::Window::Window(const WindowConfig& config) : SHV::Window(config){};

void SHV::OpenGl::Window::OnSetUpComplete() {
    AssertD(context == nullptr);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);

    context = SDL_GL_CreateContext(&GetWindow());

    if (context == nullptr) {
        throw SHV::Exception(
            "OpenGL context could not be created! SDL Error: %s\n",
            SDL_GetError());
    }
}

void SHV::OpenGl::Window::OnTearDownBegin() {
    AssertD(context != nullptr);
    SDL_GL_DeleteContext(context);
}