#pragma once

#include <memory>

struct SDL_Window;
struct SDL_Surface;

namespace SHV {
struct ImmutableConfig;
struct MutableConfig;

namespace Metal {
class Renderer;
class Window;
}  // namespace Metal

class Engine {
   public:
    Engine(const ImmutableConfig& immutableConfig,
           MutableConfig& mutableConfig);
    ~Engine();

    int Run() noexcept;

   private:
    void SetUp();
    void TearDown();

    void MainLoop();
    void Tick(float deltaTime);

    void PollEvents(float deltaTime);
    void UpdateSystems(float deltaTime);
    void RenderLoop(float deltaTime);

    const ImmutableConfig& immutableConfig;
    MutableConfig& mutableConfig;

    std::unique_ptr<Metal::Renderer> renderer;
    std::unique_ptr<Metal::Window> window;

    bool isRunning = false;
};
}  // namespace SHV
