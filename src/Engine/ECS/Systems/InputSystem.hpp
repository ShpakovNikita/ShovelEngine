#pragma once

#include "Engine/ECS/System.hpp"

#include "SDL_events.h"

namespace SHV {
class InputManager;

class InputSystem : public System {
   public:
    InputSystem(entt::registry& registry, InputManager& inputManager);
    virtual ~InputSystem();

    virtual void Process(float dt);

   private:
    InputManager& inputManager;
};
}  // namespace SHV