#pragma once

#include "SDL_metal.h"

#include "Engine/Render/WindowContext.hpp"

namespace CA {
class MetalDrawable;
}

namespace SHV {
class Window;

namespace Metal {
class WindowContext : public ::SHV::WindowContext {
   public:
    WindowContext(const Window& window);
    virtual ~WindowContext();

    const SDL_MetalView& GetMetalView() const;
    void* GetCAMetalLayer() const;

    CA::MetalDrawable* NextDrawable() const;

    virtual void SetUp();
    virtual void TearDown();

    virtual const SHV::Window& GetWindow();

   private:
    SDL_MetalView metalView;
    const SHV::Window& window;
};
}  // namespace Metal
}  // namespace SHV