#pragma once

#include <memory>

#include "Engine/Render/Renderer.hpp"

namespace SHV::OpenGl {

class Window;

class Renderer : public SHV::Renderer {
   public:
    Renderer(Window& window);
    virtual ~Renderer() = default;

    virtual void SetUp();
    virtual void TearDown();

    virtual void Draw();
};

}  // namespace SHV::OpenGl