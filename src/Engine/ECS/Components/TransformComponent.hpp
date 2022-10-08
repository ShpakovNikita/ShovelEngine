#pragma once

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace SHV {
struct TransformComponent {
    glm::mat4 matrix{1.0f};
    glm::vec3 translation;
    glm::vec3 scale{1.0f};
    glm::quat rotation;

    glm::mat4 parentWorldMatrix;

    glm::mat4 GetLocalMatrix() const {
        return glm::translate(glm::mat4(1.0f), translation) *
               glm::mat4(rotation) * glm::scale(glm::mat4(1.0f), scale) *
               matrix;
    }

    glm::mat4 GetWorldMatrix() const {
        return parentWorldMatrix * GetLocalMatrix();
    }
};
}  // namespace SHV