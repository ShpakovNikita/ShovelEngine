#pragma once

#include "Engine/ECS/System.hpp"

namespace SHV {
class MovementSystem : public System {
   public:
    MovementSystem(entt::registry& registry) : System(registry){};
    virtual ~MovementSystem() = default;

    virtual void Process(float dt);
};
}  // namespace SHV