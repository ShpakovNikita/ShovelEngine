#pragma once

#include "Engine/Core/Window.hpp"

#include "Engine/ECS/Utils/AspectRatioDelegate/AspectRatioDelegate.hpp"

namespace SHV {
class WindowAspectRatioDelegate: public AspectRatioDelegate {
   public:
    WindowAspectRatioDelegate(Window& aWindow) : window(aWindow) {}
    virtual ~WindowAspectRatioDelegate() = default;

    virtual float GetAspectRatio() {
        const glm::vec2 viewportSize = window.GetWindowSize();
        return viewportSize.x / viewportSize.y;
    }
   private:
    Window& window;
};
}  // namespace SHV