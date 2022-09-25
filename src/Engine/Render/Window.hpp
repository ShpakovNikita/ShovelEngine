#pragma once

#include <cstdint>

struct SDL_Window;

namespace SHV {
struct WindowConfig {
    const uint32_t width;
    const uint32_t height;
};

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
    const WindowConfig& config;
};
}  // namespace SHV