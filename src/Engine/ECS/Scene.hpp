#pragma once

#include <entt/entity/registry.hpp>

namespace SHV {
class Scene {
   public:
    const entt::registry& GetRegistry() const;

   private:
    entt::registry registry;
};
}  // namespace SHV
