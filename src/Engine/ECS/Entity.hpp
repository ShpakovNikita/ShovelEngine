#pragma once

#include <entt/entity/registry.hpp>

namespace SHV {
namespace Entity {
void AddChild(entt::registry& registry, entt::entity& parent,
              entt::entity& child);
}
}  // namespace SHV