#include "Engine/Core/Window.hpp"

#include <SDL.h>

#include "Engine/Common/Assert.hpp"
#include "Engine/Common/Exception.hpp"

#include "Engine/Core/Input/InputManager.hpp"

using namespace SHV;

Window::Window(const WindowConfig& aConfig) : config(aConfig){};
Window::~Window() = default;

SDL_Window& Window::GetWindow() const {
    AssertD(window != nullptr);

    return *window;
}

uint32_t Window::GetWindowID() const {
    AssertD(window != nullptr);

    return SDL_GetWindowID(window);
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

    inputManager = std::make_unique<InputManager>();

    OnSetUpComplete();
}

void Window::TearDown() {
    AssertD(window != nullptr);

    OnTearDownBegin();

    inputManager = nullptr;

    SDL_DestroyWindow(window);
}

const WindowConfig& Window::GetWindowConfig() const { return config; }

InputManager& Window::GetInputManager() {
    AssertD(inputManager != nullptr);

    return *inputManager;
}

const glm::vec2 Window::GetWindowSize() const {
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    return glm::vec2{w, h};
}

const glm::vec2 Window::GetViewportSize() const { return GetWindowSize(); }