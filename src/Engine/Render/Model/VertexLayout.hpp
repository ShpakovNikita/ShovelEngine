#pragma once

#include "glm/glm.hpp"

namespace SHV {

using Position = glm::vec4;
using Normal = glm::vec2;
using Uv = glm::vec2;

struct BasicVertexLayout {
    Position position;
    Normal normal;
    Uv uv;
};

}  // namespace SHV