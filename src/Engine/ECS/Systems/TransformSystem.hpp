#pragma once

#include "Engine/ECS/System.hpp"

namespace SHV {
class TransformSystem : public System {
   public:
    TransformSystem(entt::registry& registry) : System(registry){};
    virtual ~TransformSystem() = default;

    virtual void Process(float dt);
};
}  // namespace SHV