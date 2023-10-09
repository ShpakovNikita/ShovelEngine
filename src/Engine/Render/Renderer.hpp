#pragma once

#include <memory>
#include <vector>

#include <entt/entity/registry.hpp>

namespace SHV {

class Scene;
class Texture;

enum class eRenderApi {
    kMetal,
    kOpenGLES,
};

class Renderer {
   public:
    Renderer();
    virtual ~Renderer();

    virtual void SetUp() = 0;
    virtual void TearDown() = 0;

    virtual void SetUpScene(Scene& scene) const = 0;
    virtual void TearDownScene(Scene& scene) const = 0;

    virtual void Draw(const Scene& scene) = 0;
    virtual std::shared_ptr<Texture> Draw(const Scene& scene, const Texture& renderTargetPrototype) = 0;

    virtual void BeginFrame(){};
    virtual void EndFrame(){};

    virtual void WaitForFrameExecutionFinish(){};
    virtual std::vector<entt::entity> SortedRenderQueue(const Scene& scene);
};
}  // namespace SHV