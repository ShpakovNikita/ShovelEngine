#pragma once

#include <glm/vec2.hpp>

#include "Engine/Render/WindowContext.hpp"

using SDL_GLContext = void*;

namespace SHV {
class Window;

namespace OpenGl {
class WindowContext : public ::SHV::WindowContext {
   public:
    WindowContext(const SHV::Window& window);

    SDL_GLContext GetGlContext();

    virtual void SetUp();
    virtual void TearDown();

    virtual const SHV::Window& GetWindow();

    const glm::vec2 GetViewportSize() const;

   private:
    SDL_GLContext context = nullptr;
    const Window& window;
};
}  // namespace OpenGl
}  // namespace SHV