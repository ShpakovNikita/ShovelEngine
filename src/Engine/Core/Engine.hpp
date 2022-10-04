#pragma once

#include <memory>

namespace SHV {
struct ImmutableConfig;
struct MutableConfig;

class RenderContext;
class ImGui;
class Toolbar;
class Scene;

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
    std::unique_ptr<ImGui> imgui;
    std::unique_ptr<Toolbar> toolbar;
    std::unique_ptr<Scene> scene;

    bool isRunning = false;
};
}  // namespace SHV
