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

        if (inputComponent.input.IsKeyPressed(eKey::kW)) {
            transformComponent.translation +=
                cameraComponent.cameraFront * movementComponent.speed * dt;
        }
        if (inputComponent.input.IsKeyPressed(eKey::kA)) {
            transformComponent.translation -=
                glm::normalize(glm::cross(cameraComponent.cameraFront,
                                          cameraComponent.cameraUp)) *
                movementComponent.speed * dt;
        }
        if (inputComponent.input.IsKeyPressed(eKey::kS)) {
            transformComponent.translation -=
                cameraComponent.cameraFront * movementComponent.speed * dt;
        }
        if (inputComponent.input.IsKeyPressed(eKey::kD)) {
            transformComponent.translation +=
                glm::normalize(glm::cross(cameraComponent.cameraFront,
                                          cameraComponent.cameraUp)) *
                movementComponent.speed * dt;
        }
    }
}