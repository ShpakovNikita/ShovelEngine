#pragma once

#include <entt/entity/registry.hpp>

namespace SHV {
struct Primitive;

namespace ObjectCreationUtils {
entt::entity CreateCube(entt::registry& registry, float edgeSize = 1.0f);
Primitive CreateCubePrimitive(float edgeSize = 1.0f);
}
}  // namespace SHV
