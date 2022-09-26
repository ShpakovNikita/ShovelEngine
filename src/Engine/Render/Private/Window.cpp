#include "Engine/Render/Window.hpp"

#include <SDL.h>

#include "Engine/Common/Exception.hpp"
#include "Engine/Utils/Assert.hpp"

using namespace SHV;

Window::Window(const WindowConfig& aConfig) : config(aConfig){};
Window::~Window() = default;

SDL_Window& Window::GetWindow() const {
    AssertD(window != nullptr);

    return *window;
}

void Window::SetUp() {
    AssertD(window == nullptr);

    // Create window
    window = SDL_CreateWindow(
        "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        config.width, config.height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

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
