#pragma once

#include <memory>
#include <optional>

#include "Engine/Core/MutableConfig.hpp"

namespace SHV {
enum class eRenderApi;

struct ImmutableConfig;
struct MutableConfig;

class RenderContext;
class Toolbar;
class Scene;

class Engine {
   public:
    Engine(const ImmutableConfig& immutableConfig,
           MutableConfig& mutableConfig);
    ~Engine();

    int Run() noexcept;

    const MutableConfig& GetMutableConfig() const;
    void SetMutableConfig(MutableConfig& config);

   private:
    void CreateCharacter();

    void CreateScene();
    void DestroyScene();

    void LoadPrimitives();
    void UnloadPrimitives();

    void SetUp();
    void TearDown();

    void MainLoop();
    void Tick(float deltaTime);

    void PollEvents(float deltaTime);
    void UpdateSystems(float deltaTime);
    void RenderLoop(float deltaTime);

    void HandleMutableConfigChange();

    const ImmutableConfig& immutableConfig;
    
    MutableConfig mutableConfig;
    std::optional<MutableConfig> newMutableConfig;

    std::unique_ptr<RenderContext> renderContext;
    std::unique_ptr<Toolbar> toolbar;
    std::unique_ptr<Scene> scene;

    bool isRunning = false;
};
}  // namespace SHV
