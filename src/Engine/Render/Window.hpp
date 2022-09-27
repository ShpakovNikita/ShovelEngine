#pragma once

#include <string>

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

   private:
    virtual void OnSetUpComplete(){};
    virtual void OnTearDownBegin(){};

    SDL_Window* window = nullptr;
    const WindowConfig config;
};
}  // namespace SHV