#pragma once

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "TransformComponent.hpp"

namespace SHV {
struct CameraComponent {
    bool isActive = true;

    float sensitivity = 5.0f;

    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 projection{1.0f};

    glm::mat4 GetViewMatrix(
        const TransformComponent& transformComponent) const {
        const glm::vec3 cameraFrontRotated =
            transformComponent.rotation * cameraFront;
        const glm::vec3 cameraUpRotated =
            transformComponent.rotation * cameraUp;
        return glm::lookAt(transformComponent.translation,
                           transformComponent.translation + cameraFrontRotated,
                           cameraUpRotated);
    }
};
}  // namespace SHV