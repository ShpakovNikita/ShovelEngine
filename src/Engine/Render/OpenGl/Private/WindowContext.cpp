#include "Engine/Render/OpenGl/WindowContext.hpp"

#include <SDL_video.h>

#include "Engine/Core/Window.hpp"

#include "Engine/Common/Assert.hpp"
#include "Engine/Common/Exception.hpp"

using namespace SHV;

OpenGl::WindowContext::WindowContext(const SHV::Window& aWindow)
    : SHV::WindowContext(), window(aWindow){};

void OpenGl::WindowContext::SetUp() {
    AssertD(context == nullptr);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);

    context = SDL_GL_CreateContext(&window.GetWindow());

    if (context == nullptr) {
        throw SHV::Exception(
            "OpenGL context could not be created! SDL Error: %s\n",
            SDL_GetError());
    }
}

void OpenGl::WindowContext::TearDown() {
    AssertD(context != nullptr);
    SDL_GL_DeleteContext(context);
}

SDL_GLContext OpenGl::WindowContext::GetGlContext() { return context; }

const SHV::Window& OpenGl::WindowContext::GetWindow() { return window; }

const glm::vec2 OpenGl::WindowContext::GetViewportSize() const {
    int w, h;
    SDL_GL_GetDrawableSize(&window.GetWindow(), &w, &h);
    return glm::vec2{w, h};
}