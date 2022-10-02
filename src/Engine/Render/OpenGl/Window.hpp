#pragma once

#include "Engine/Render/Window.hpp"

using SDL_GLContext = void*;

struct SDL_VideoInfo;

namespace SHV {
namespace OpenGl {
class Window : public ::SHV::Window {
   public:
    Window(const WindowConfig& config);

   private:
    virtual void OnSetUpComplete();
    virtual void OnTearDownBegin();

    SDL_GLContext context;
};
}  // namespace OpenGl
}  // namespace SHV