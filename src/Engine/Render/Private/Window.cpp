#include "Engine/Render/Window.hpp"

#include <SDL.h>

#include "Engine/Common/Assert.hpp"
#include "Engine/Common/Exception.hpp"

using namespace SHV;

Window::Window(const WindowConfig& aConfig) : config(aConfig){};
Window::~Window() = default;

SDL_Window& Window::GetWindow() const {
    AssertD(window != nullptr);

    return *window;
}

void Window::SetUp() {
    AssertD(window == nullptr);

    auto windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

    switch (config.renderApi) {
        case eRenderApi::kOpenGLES:
            windowFlags |= SDL_WINDOW_OPENGL;
            break;
        case eRenderApi::kMetal:
            windowFlags |= SDL_WINDOW_METAL;
            break;
    }

    // Create window
    window = SDL_CreateWindow(config.name.data(), SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, config.width,
                              config.height, windowFlags);

    if (window == nullptr) {
        throw SHV::Exception("Window could not be created! SDL_Error: %s\n",
                             SDL_GetError());
    }

    OnSetUpComplete();
}

void Window::TearDown() {
    AssertD(window != nullptr);

    OnTearDownBegin();

    SDL_DestroyWindow(window);
}

const WindowConfig& Window::GetWindowConfig() const { return config; }
