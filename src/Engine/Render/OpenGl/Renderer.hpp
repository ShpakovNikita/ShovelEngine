#pragma once

#include <GL/glew.h>

#include <memory>
#include <optional>
#include <string>

#include "Engine/Render/Renderer.hpp"

namespace SHV::OpenGl {

class Window;
class ShaderProgram;
class RenderBatch;

class Renderer : public SHV::Renderer {
   public:
    Renderer(Window& window);
    virtual ~Renderer();

    virtual void SetUp();
    virtual void TearDown();

    virtual void Draw(const Scene& scene);

    virtual void BeginFrame();
    virtual void EndFrame();

   private:
    std::optional<std::string> ValidateExtensions();

    std::unique_ptr<ShaderProgram> program;

    Window& window;
};

}  // namespace SHV::OpenGl