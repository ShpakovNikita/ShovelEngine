#include "Engine/Tools/Scene/GltfSceneLoader.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "tiny_gltf.h"

#include "Engine/ECS/Scene.hpp"
#include "Engine/Common/Exception.hpp"
#include "Engine/Common/Logger.hpp"

using namespace SHV;

void SHV::LoadGltfModelToScene(Scene& /*scene*/,
                               const std::string& modelFilePath) {
    LogI(eTag::kAssetLoading)
        << "Start loading glTF model: " << modelFilePath << std::endl;

    std::string extension =
        modelFilePath.substr(modelFilePath.find_last_of(".") + 1);

    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    bool success = false;

    if (extension == "glb") {
        success = loader.LoadBinaryFromFile(&model, &err, &warn, modelFilePath);
    } else if (extension == "gltf") {
        success = loader.LoadASCIIFromFile(&model, &err, &warn, modelFilePath);
    }

    if (!success) {
        throw Exception("Failed to parse glTF %s", modelFilePath.c_str());
    }

    if (!warn.empty()) {
        LogW(eTag::kAssetLoading)
            << "glTF loaded with warnings: " << warn << std::endl;
    }

    if (!err.empty()) {
        throw Exception("Failed to load glTF %s with error %s",
                        modelFilePath.c_str(), err.c_str());
    }

    LogI(eTag::kAssetLoading)
        << "glTF model loaded successfully: " << modelFilePath << std::endl;
}