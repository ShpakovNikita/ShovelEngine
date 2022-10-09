#pragma once

#include <memory>

namespace SHV {

class Scene;

enum eRenderApi {
    kMetal,
    kOpenGLES,
};

class Renderer {
   public:
    Renderer();
    virtual ~Renderer();

    virtual void SetUp() = 0;
    virtual void TearDown() = 0;

    virtual void SetUpScene(Scene& scene) = 0;
    virtual void TearDownScene(Scene& scene) = 0;

    virtual void Draw(const Scene& scene) = 0;
    virtual void BeginFrame(){};
    virtual void EndFrame(){};
};
}  // namespace SHV