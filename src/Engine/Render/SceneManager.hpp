#pragma once

#include <map>
#include <memory>
#include <functional>

#include "Engine/Common/Assert.hpp"
#include "Engine/ECS/Scene.hpp"

/**
 * This class is used to create and track specific graphics API renderable scene
 * data, according to used scenes.
 *
 * It uses ECS to collect scene data
 */
namespace SHV {
template <class RenderScene>
class SceneManager {
   public:
    SceneManager();

    void ProcessScene(const Scene& scene);
    RenderScene& GetRenderScene(const Scene& scene) const;

   private:
    std::map<const Scene*, std::unique_ptr<RenderScene>> processedScenes;
};

template <class RenderScene>
void SceneManager<RenderScene>::ProcessScene(const Scene& scene) {
    auto sceneIt = processedScenes.find(scene);
    if (sceneIt == processedScenes.end()) {
        processedScenes.insert({scene, std::make_unique<RenderScene>(scene)});
    }
}

template <class RenderScene>
RenderScene& SceneManager<RenderScene>::GetRenderScene(
    const Scene& scene) const {
    auto sceneIt = processedScenes.find(scene);
    AssertD(sceneIt != processedScenes.end());
    return *sceneIt->second;
}

}  // namespace SHV