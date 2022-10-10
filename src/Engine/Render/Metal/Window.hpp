#pragma once

#include "Engine/Core/Window.hpp"
#include "SDL_metal.h"

namespace CA {
class MetalDrawable;
}

namespace SHV {
namespace Metal {
class Window : public ::SHV::Window {
   public:
    Window(const WindowConfig& config);

    const SDL_MetalView& GetMetalView() const;
    void* GetCAMetalLayer() const;

    CA::MetalDrawable* NextDrawable() const;

   private:
    virtual void OnSetUpComplete();
    virtual void OnTearDownBegin();

    SDL_MetalView metalView;
};
}  // namespace Metal
}  // namespace SHV