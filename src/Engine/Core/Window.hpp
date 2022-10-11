#pragma once

#include <string>
#include <glm/vec2.hpp>

#include "Engine/Render/WindowConfig.hpp"

struct SDL_Window;

namespace SHV {

class InputManager;

class Window final {
   public:
    Window(const WindowConfig& config);
    ~Window();

    SDL_Window& GetWindow() const;
    uint32_t GetWindowID() const;

    void SetUp();
    void TearDown();

    const WindowConfig& GetWindowConfig() const;
    const glm::vec2 GetWindowSize() const;

    InputManager& GetInputManager();

   private:
    SDL_Window* window = nullptr;
    const WindowConfig config;

    std::unique_ptr<InputManager> inputManager;
};
}  // namespace SHV