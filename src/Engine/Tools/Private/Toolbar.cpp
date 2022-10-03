#include "Engine/Tools/Toolbar.hpp"

#include "imgui.h"
#include "ImGuiFileDialog.h"

#include <string>

namespace SHV::SToolbar {
std::string GetSampleModelsDir() {
#if defined(SAMPLE_MODELS_DIR)
    return SAMPLE_MODELS_DIR;
#else
    return ".";
#endif
}
}  // namespace SHV::SToolbar

void SHV::Toolbar::Draw() {
    // ::ImGui::ShowDemoWindow();

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Add model to scene")) {
                std::string sampleModelsDir =
                    SHV::SToolbar::GetSampleModelsDir();
                ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey",
                                                        "Choose File", ".gltf",
                                                        sampleModelsDir);
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    // display
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
        // action if OK
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePathName =
                ImGuiFileDialog::Instance()->GetFilePathName();
            std::string filePath =
                ImGuiFileDialog::Instance()->GetCurrentPath();
            // action
        }

        // close
        ImGuiFileDialog::Instance()->Close();
    }
}
