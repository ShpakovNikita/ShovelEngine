#include "Engine/Tools/Scene/CameraDataViewer.hpp"

#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/ECS/Components/CameraComponent.hpp"
#include "Engine/ECS/Components/InputComponent.hpp"
#include "Engine/ECS/Components/MovementComponent.hpp"

#include "Engine/Common/Assert.hpp"

#include "imgui.h"

using namespace SHV;

CameraDataViewer::CameraDataViewer(entt::registry& aRegistry)
    : registry(aRegistry) {}

void CameraDataViewer::Draw(bool* show) {
    if (!ImGui::Begin("Camera data viewer", show,
                      ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar |
                          ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav |
                          ImGuiWindowFlags_NoScrollbar)) {
        ImGui::End();
        return;
    }

    auto cameraView = registry.view<CameraComponent, TransformComponent,
                                    InputComponent, MovementComponent>();

    for (auto&& [cameraEntity, cameraComponent, transformComponent,
                 inputComponent, movementComponent] : cameraView.each()) {
        if (cameraComponent.isActive) {
            float cameraPos[3] = {transformComponent.translation.x,
                                  transformComponent.translation.y,
                                  transformComponent.translation.z};

            const auto eulerRotation = transformComponent.rotation;

            float cameraYawPitch[2] = {
                glm::degrees(eulerRotation.y),
                glm::degrees(eulerRotation.x),
            };

            float cameraFow[1] = {glm::degrees(cameraComponent.cameraFOV)};

            ImGui::InputFloat3("Camera pos", cameraPos);
            ImGui::InputFloat2("Camera yaw pitch", cameraYawPitch);
            ImGui::InputFloat("Camera FOV", cameraFow);
            ImGui::InputFloat("Camera Speed", &movementComponent.speed);

            cameraComponent.cameraFOV = glm::radians(cameraFow[0]);
        }
    }

    ImGui::End();
}