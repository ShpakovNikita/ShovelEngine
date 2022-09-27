#pragma once

#include <memory>

namespace SHV {
struct ImmutableConfig;
struct MutableConfig;

class RenderContext;

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

    std::unique_ptr<RenderContext> renderContext;

    bool isRunning = false;
};
}  // namespace SHV
