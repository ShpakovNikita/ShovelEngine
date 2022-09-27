#pragma once

namespace SHV {

enum eRenderApi {
    kMetal,
    kOpenGLES,
};

class Renderer {
   public:
    Renderer() = default;
    virtual ~Renderer() = default;

    virtual void SetUp() = 0;
    virtual void TearDown() = 0;

    virtual void Draw() = 0;
};
}  // namespace SHV