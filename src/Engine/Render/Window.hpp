#pragma once

#include <string>
#include <glm/vec2.hpp>

#include "WindowConfig.hpp"

struct SDL_Window;

namespace SHV {
class Window {
   public:
    Window(const WindowConfig& config);
    virtual ~Window();

    SDL_Window& GetWindow() const;

    void SetUp();
    void TearDown();

    const WindowConfig& GetWindowConfig() const;
    const glm::vec2 GetWindowSize() const;
    virtual const glm::vec2 GetViewportSize() const;

   private:
    virtual void OnSetUpComplete(){};
    virtual void OnTearDownBegin(){};

    SDL_Window* window = nullptr;
    const WindowConfig config;
};
}  // namespace SHV