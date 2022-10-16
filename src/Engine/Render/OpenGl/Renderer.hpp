#pragma once

#include <GL/glew.h>

#include <memory>
#include <optional>
#include <string>

#include "Engine/Render/Renderer.hpp"

namespace SHV::OpenGl {

class WindowContext;
class ShaderProgram;
class RenderBatch;

class Renderer : public SHV::Renderer {
   public:
    Renderer(WindowContext& window);
    virtual ~Renderer();

    virtual void SetUp();
    virtual void TearDown();

    virtual void Draw(const Scene& scene);

    virtual void SetUpScene(Scene& scene);
    virtual void TearDownScene(Scene& scene);

    virtual void BeginFrame();
    virtual void EndFrame();

    virtual void WaitForFrameExecutionFinish();

   private:
    std::optional<std::string> ValidateExtensions();

    std::unique_ptr<ShaderProgram> program;

    WindowContext& windowContext;
};

}  // namespace SHV::OpenGl