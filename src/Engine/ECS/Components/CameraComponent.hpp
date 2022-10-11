#pragma once

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace SHV {
struct CameraComponent {
    bool isActive = true;

    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 projection{1.0f};
};
}  // namespace SHV