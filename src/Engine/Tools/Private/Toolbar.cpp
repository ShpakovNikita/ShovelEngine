#include "Engine/Tools/Toolbar.hpp"

#include "imgui.h"
#include "ImGuiFileDialog.h"

#include "Engine/Core/Engine.hpp"

#include "Engine/ECS/Scene.hpp"
#include "Engine/Tools/Scene/GltfSceneLoader.hpp"
#include "Engine/Tools/Scene/HierarchyViewer.hpp"
#include "Engine/Tools/Scene/CameraDataViewer.hpp"
#include "Engine/Tools/Profiling/ImGuiProfilerRenderer.hpp"

namespace SHV::SToolbar {
std::string GetSampleModelsDir() {
#if defined(SAMPLE_MODELS_DIR)
    return SAMPLE_MODELS_DIR;
#else
    return ".";
#endif
}
}  // namespace SHV::SToolbar

SHV::Toolbar::Toolbar(SHV::Scene& aScene, Engine& aEngine)
    : scene(aScene),
      engine(aEngine),
      hierarchyViewer(std::make_unique<HierarchyViewer>(aScene.GetRegistry())),
      cameraDataViewer(
          std::make_unique<CameraDataViewer>(aScene.GetRegistry())),
      profilersDataViewer(std::make_unique<ProfilersWindow>()) {}

SHV::Toolbar::~Toolbar() {}

void SHV::Toolbar::Draw() {
    if (ImGui::BeginMainMenuBar()) {
        DrawFileMenu();
        DrawToolsMenu();
        DrawEngineMenu();

        ImGui::EndMainMenuBar();
    }

    if (showHierarchyViewer) {
        hierarchyViewer->Draw(&showHierarchyViewer);
    }

    if (showCameraDataViewer) {
        cameraDataViewer->Draw(&showCameraDataViewer);
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
        ImGui::Checkbox("Camera Data", &showCameraDataViewer);
        ImGui::Checkbox("Profilers Data", &showProfilersDataViewer);
        ImGui::EndMenu();
    }
}

void SHV::Toolbar::DrawEngineMenu() {
    if (ImGui::BeginMenu("Engine")) {
        MutableConfig config = engine.GetMutableConfig();

        if (ImGui::RadioButton("Metal Api",
                               config.renderApi == eRenderApi::kMetal)) {
            config.renderApi = eRenderApi::kMetal;
            engine.SetMutableConfig(config);
        }
        if (ImGui::RadioButton("OpenGL ES",
                               config.renderApi == eRenderApi::kOpenGLES)) {
            config.renderApi = eRenderApi::kOpenGLES;
            engine.SetMutableConfig(config);
        }
        ImGui::EndMenu();
    }
}
