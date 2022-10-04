#pragma once

#include <string>

namespace SHV {
class Scene;

void LoadGltfModelToScene(Scene& scene, const std::string& modelFilePath);
}  // namespace SHV