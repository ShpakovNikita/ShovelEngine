#pragma once

#include <GL/glew.h>

#include <memory>

#include "Engine/Render/Renderer.hpp"

namespace SHV::OpenGl {

class Window;
class ShaderProgram;

class Renderer : public SHV::Renderer {
   public:
    Renderer(Window& window);
    virtual ~Renderer();

    virtual void SetUp();
    virtual void TearDown();

    virtual void Draw();

   private:
    std::unique_ptr<ShaderProgram> program;
};

}  // namespace SHV::OpenGl