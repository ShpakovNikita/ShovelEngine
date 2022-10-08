#include "Engine/Tools/Scene/GltfSceneLoader.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "tiny_gltf.h"

#include "Engine/Render/Model/Material.hpp"
#include "Engine/ECS/Components/RenderComponent.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/ECS/Scene.hpp"
#include "Engine/ECS/Entity.hpp"
#include "Engine/Common/Exception.hpp"
#include "Engine/Common/Logger.hpp"

using namespace SHV;

namespace SHV::SLoadGltfModelToScene {
void CreateTransformComponent(entt::registry& registry, entt::entity& entity,
                              const tinygltf::Node& node) {
    auto& transformComponent = registry.emplace<TransformComponent>(entity);

    // Generate local node matrix
    if (node.translation.size() == 3) {
        glm::vec3 translation = glm::make_vec3(node.translation.data());
        transformComponent.translation = translation;
    }

    if (node.rotation.size() == 4) {
        glm::quat q = glm::make_quat(node.rotation.data());
        transformComponent.rotation = glm::mat4(q);
    }

    if (node.scale.size() == 3) {
        glm::vec3 scale = glm::make_vec3(node.scale.data());
        transformComponent.scale = scale;
    }

    if (node.matrix.size() == 16) {
        transformComponent.matrix = glm::make_mat4x4(node.matrix.data());
    };
}

void CreateRenderComponent(entt::registry& registry, entt::entity& entity,
                           const tinygltf::Node& node,
                           const tinygltf::Model& model) {
    auto& renderComponent = registry.emplace<RenderComponent>(entity);
    renderComponent.primitive.material = std::make_shared<Material>();

    // Node contains mesh data
    if (node.mesh > -1) {
        const tinygltf::Mesh mesh = model.meshes[node.mesh];

        for (size_t j = 0; j < mesh.primitives.size(); j++) {
            // const tinygltf::Primitive& primitive = mesh.primitives[j];
            // TODO: mesh primitive filling
        }
    }
}

void LoadNode(entt::registry& registry, entt::entity& parent,
              const tinygltf::Node& node, int /*nodeIndex*/,
              const tinygltf::Model& model) {
    auto entityNode = registry.create();

    CreateTransformComponent(registry, entityNode, node);
    CreateRenderComponent(registry, entityNode, node, model);

    // Node with children
    if (node.children.size() > 0) {
        for (size_t i = 0; i < node.children.size(); i++) {
            LoadNode(registry, entityNode, model.nodes[node.children[i]],
                     node.children[i], model);
        }
    }

    Entity::AddChild(registry, parent, entityNode);
}
}  // namespace SHV::SLoadGltfModelToScene

void SHV::LoadGltfModelToScene(Scene& scene, const std::string& modelFilePath) {
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

    if (model.scenes.empty()) {
        throw Exception("Could not the load file! Model scenes are empty!");
    }

    auto rootNode = scene.GetRegistry().create();

    const tinygltf::Scene& gltfScene =
        model.scenes[model.defaultScene > -1 ? model.defaultScene : 0];
    for (size_t i = 0; i < gltfScene.nodes.size(); i++) {
        const tinygltf::Node node = model.nodes[gltfScene.nodes[i]];
        SLoadGltfModelToScene::LoadNode(scene.GetRegistry(), rootNode, node,
                                        gltfScene.nodes[i], model);
    }

    Entity::AddChild(scene.GetRegistry(), scene.GetRootEntity(), rootNode);

    LogI(eTag::kAssetLoading)
        << "glTF model loaded successfully: " << modelFilePath << std::endl;
}