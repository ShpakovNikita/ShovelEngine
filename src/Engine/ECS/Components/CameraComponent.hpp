#pragma once

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace SHV {
struct CameraComponent {
    bool isActive = true;

    float sensitivity = 5.0f;

    const glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
    const glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float cameraFOV = glm::radians(45.0f);

    glm::mat4 projection{1.0f};

    glm::vec3 cameraRotation = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::mat4 GetViewMatrix(const glm::vec3& translation) const {
        const glm::quat cameraRotQuat = cameraRotation;
        const glm::vec3 cameraFrontRotated = cameraRotQuat * cameraFront;
        const glm::vec3 cameraUpRotated = cameraRotQuat * cameraUp;
        return glm::lookAt(translation, translation + cameraFrontRotated,
                           cameraUpRotated);
    }
};
}  // namespace SHV