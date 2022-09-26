#pragma once

namespace SHV {

enum eRenderApi {
    kMetal,
    kOpenGLES,
};

class Renderer {
   public:
    Renderer(const eRenderApi& renderApi);

    void SetUp();
    void TearDown();

   private:
    const eRenderApi& renderApi;
};
}  // namespace SHV