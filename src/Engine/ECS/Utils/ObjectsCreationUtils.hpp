#pragma once

#include <entt/entity/registry.hpp>

namespace SHV {
struct Primitive;

namespace ObjectCreationUtils {
entt::entity CreateCube(entt::registry& registry);
Primitive CreateCubePrimitive();
}
}  // namespace SHV
