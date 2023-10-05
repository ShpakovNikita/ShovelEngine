#include "Engine/ECS/Systems/MovementSystem.hpp"

#include "Engine/ECS/Components/MovementComponent.hpp"
#include "Engine/ECS/Components/InputComponent.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/ECS/Components/CameraComponent.hpp"

#include <Tracy.hpp>
#include "Engine/Common/ProfilerSystems.hpp"

using namespace SHV;

void MovementSystem::Process(float dt) {
    ZoneNamedN(
        __tracy, "MovementSystem Process",
        static_cast<bool>(kActiveProfilerSystems & ProfilerSystems::ECS));

    auto renderView =
        registry.view<SHV::InputComponent, SHV::MovementComponent,
                      SHV::CameraComponent, SHV::TransformComponent>();

    for (auto&& [entity, inputComponent, movementComponent, cameraComponent,
                 transformComponent] : renderView.each()) {
        if (!movementComponent.isActive) {
            continue;
        }

        if (inputComponent.input.IsKeyPressed(eKey::kRMB) &&
            inputComponent.normalizedMotion.length() != 0) {
            /*
            auto transformEulerAngles =
                eulerAngles(transformComponent.rotation);

            transformEulerAngles.x += inputComponent.normalizedMotion.y *
                                      cameraComponent.sensitivity;  // pitch
            transformEulerAngles.y -= inputComponent.normalizedMotion.x *
                                      cameraComponent.sensitivity;  // yaw

            if (transformEulerAngles.x > glm::radians(89.0f)) {
                transformEulerAngles.x = glm::radians(89.0f);
            }

            if (transformEulerAngles.x < glm::radians(-89.0f)) {
                transformEulerAngles.x = glm::radians(-89.0f);
            }

            transformComponent.rotation = glm::quat(transformEulerAngles);
             */

            auto& rotation = cameraComponent.cameraRotation;

            rotation.x += inputComponent.normalizedMotion.y *
                          cameraComponent.sensitivity;  // pitch
            rotation.y -= inputComponent.normalizedMotion.x *
                          cameraComponent.sensitivity;  // yaw

            if (rotation.x > glm::radians(89.0f)) {
                rotation.x = glm::radians(89.0f);
            }

            if (rotation.x < glm::radians(-89.0f)) {
                rotation.x = glm::radians(-89.0f);
            }
        }

        const glm::vec3 cameraDirection = glm::normalize(
            glm::quat(
                cameraComponent.cameraRotation) /*transformComponent.rotation*/
            * cameraComponent.cameraFront);

        if (inputComponent.input.IsKeyPressed(eKey::kW)) {
            transformComponent.translation +=
                cameraDirection * movementComponent.speed * dt;
        }
        if (inputComponent.input.IsKeyPressed(eKey::kA)) {
            transformComponent.translation -=
                glm::normalize(
                    glm::cross(cameraDirection, cameraComponent.cameraUp)) *
                movementComponent.speed * dt;
        }
        if (inputComponent.input.IsKeyPressed(eKey::kS)) {
            transformComponent.translation -=
                cameraDirection * movementComponent.speed * dt;
        }
        if (inputComponent.input.IsKeyPressed(eKey::kD)) {
            transformComponent.translation +=
                glm::normalize(
                    glm::cross(cameraDirection, cameraComponent.cameraUp)) *
                movementComponent.speed * dt;
        }
    }
}