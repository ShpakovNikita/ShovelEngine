#include "Engine/Tools/Toolbar.hpp"

#include "imgui.h"
#include "ImGuiFileDialog.h"

#include "Engine/ECS/Scene.hpp"
#include "Engine/Tools/Scene/GltfSceneLoader.hpp"
#include "Engine/Tools/Scene/HierarchyViewer.hpp"

namespace SHV::SToolbar {
std::string GetSampleModelsDir() {
#if defined(SAMPLE_MODELS_DIR)
    return SAMPLE_MODELS_DIR;
#else
    return ".";
#endif
}
}  // namespace SHV::SToolbar

SHV::Toolbar::Toolbar(SHV::Scene& aScene)
    : scene(aScene),
      hierarchyViewer(std::make_unique<HierarchyViewer>(aScene.GetRegistry())) {
}

SHV::Toolbar::~Toolbar() {}

void SHV::Toolbar::Draw() {
    if (ImGui::BeginMainMenuBar()) {
        DrawFileMenu();
        DrawToolsMenu();

        ImGui::EndMainMenuBar();
    }

    if (showHierarchyViewer) {
        hierarchyViewer->Draw(&showHierarchyViewer);
    }

    // display
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
        // action if OK
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePathName =
                ImGuiFileDialog::Instance()->GetFilePathName();

            SHV::LoadGltfModelToScene(scene, filePathName);
        }

        // close
        ImGuiFileDialog::Instance()->Close();
    }
}

void SHV::Toolbar::DrawFileMenu() {
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Add model to scene")) {
            std::string sampleModelsDir = SHV::SToolbar::GetSampleModelsDir();
            ImGuiFileDialog::Instance()->OpenDialog(
                "ChooseFileDlgKey", "Choose File", ".gltf", sampleModelsDir);
        }
        ImGui::EndMenu();
    }
}

void SHV::Toolbar::DrawToolsMenu() {
    if (ImGui::BeginMenu("Tools")) {
        ImGui::Checkbox("Hierarchy", &showHierarchyViewer);
        ImGui::EndMenu();
    }
}
