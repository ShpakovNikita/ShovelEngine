#pragma once

#include <entt/entity/registry.hpp>

namespace SHV {
class System {
   public:
    System(entt::registry& registry);
    virtual ~System() = default;

    virtual void Process(float dt) = 0;

    virtual void SetUp(){};
    virtual void TearDown(){};

   protected:
    entt::registry& registry;
};
}  // namespace SHV