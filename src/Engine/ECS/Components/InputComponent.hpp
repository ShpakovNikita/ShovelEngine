#pragma once

#include "Engine/Core/Input/Input.hpp"
#include <glm/vec2.hpp>

namespace SHV {

struct InputComponent {
    Input input;
    glm::vec2 normalizedMotion;
};
}  // namespace SHV