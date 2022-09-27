#pragma once

#include "Engine/Render/Window.hpp"

namespace SHV {
namespace OpenGl {
class Window : public ::SHV::Window {
   public:
    Window(const WindowConfig& config);

   private:
    virtual void OnSetUpComplete();
    virtual void OnTearDownBegin();
};
}  // namespace OpenGl
}  // namespace SHV