#include "Engine/Core/Window.hpp"

#include <SDL.h>

#include "Engine/Common/Assert.hpp"
#include "Engine/Common/Exception.hpp"

#include "Engine/Core/Input/InputManager.hpp"

using namespace SHV;

Window::Window(const WindowConfig& aConfig) : config(aConfig){
    inputManager = std::make_unique<InputManager>();
};
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

    // Create window
    window = SDL_CreateWindow(config.name.data(), SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, config.width,
                              config.height, 0);

    if (window == nullptr) {
        throw SHV::Exception("Window could not be created! SDL_Error: %s\n",
                             SDL_GetError());
    }
    
    inputManager->SetUp();
}

void Window::TearDown() {
    AssertD(window != nullptr);
    
    inputManager->TearDown();

    SDL_DestroyWindow(window);
    window = nullptr;
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
