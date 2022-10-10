#pragma once

#include "Engine/ECS/System.hpp"

namespace SHV {
class Window;

class CameraSystem : public System {
   public:
    CameraSystem(entt::registry& registry, Window& window);
    virtual ~CameraSystem();

    virtual void Process(float dt);

   private:
    Window& window;
};
}  // namespace SHV