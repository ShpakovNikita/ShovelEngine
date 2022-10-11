#include "Engine/Tools/Scene/CameraDataViewer.hpp"

#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/ECS/Components/CameraComponent.hpp"
#include "Engine/ECS/Components/InputComponent.hpp"

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

    auto cameraView =
        registry.view<CameraComponent, TransformComponent, InputComponent>();

    for (auto&& [cameraEntity, cameraComponent, transformComponent,
                 inputComponent] : cameraView.each()) {
        if (cameraComponent.isActive) {
            float cameraPos[3] = {transformComponent.translation.x,
                                  transformComponent.translation.y,
                                  transformComponent.translation.z};
            ImGui::InputFloat3("Camera pos", cameraPos);
        }
    }

    ImGui::End();
}