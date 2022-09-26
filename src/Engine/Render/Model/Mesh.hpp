#pragma once

#include <vector>

#include "Primitive.hpp"

namespace SHV {
class Mesh {
   public:
    std::vector<Primitive> primitives;
};
}  // namespace SHV