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
    // TODO: remove;
    void LoadPrimitives();
    void UnloadPrimitives();

    std::optional<std::string> ValidateExtensions();

    std::shared_ptr<RenderBatch> renderBatch;
    std::unique_ptr<ShaderProgram> program;

    Window& window;
};

}  // namespace SHV::OpenGl