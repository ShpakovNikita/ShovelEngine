#pragma once

#include <memory>

struct SDL_Window;
struct SDL_Surface;

namespace SHV {
struct ImmutableConfig;

namespace Metal {
class Renderer;
class Window;
}

class Engine {
   public:
    Engine(const ImmutableConfig& config);
    ~Engine();

    int Run() noexcept;

   private:
    void SetUp();
    void TearDown();

    const ImmutableConfig& config;

    SDL_Surface* screenSurface = nullptr;

    std::unique_ptr<Metal::Renderer> renderer;
    std::unique_ptr<Metal::Window> window;
};
}  // namespace SHV
