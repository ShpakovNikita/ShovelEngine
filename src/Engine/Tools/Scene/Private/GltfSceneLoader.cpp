#include "Engine/Tools/Scene/GltfSceneLoader.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "tiny_gltf.h"

#include "Engine/Render/Model/Material.hpp"
#include "Engine/Render/ECS/Components/RenderComponent.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/ECS/Scene.hpp"
#include "Engine/ECS/Entity.hpp"
#include "Engine/Common/Exception.hpp"
#include "Engine/Common/Assert.hpp"
#include "Engine/Common/Logger.hpp"

using namespace SHV;

namespace SHV::SLoadGltfModelToScene {
template <typename T>
void FillVertexAttribute(const tinygltf::Primitive& primitive,
                         const tinygltf::Model& input,
                         const std::string& attrName, int compSize,
                         const T** buffer, int& bufferStride) {
    if (primitive.attributes.find(attrName) != primitive.attributes.end()) {
        const tinygltf::Accessor& accessor =
            input.accessors[primitive.attributes.find(attrName)->second];
        const tinygltf::BufferView& view =
            input.bufferViews[accessor.bufferView];
        *buffer = reinterpret_cast<const T*>(
            &(input.buffers[view.buffer]
                  .data[accessor.byteOffset + view.byteOffset]));
        bufferStride = accessor.ByteStride(view)
                           ? (accessor.ByteStride(view) / sizeof(T))
                           : tinygltf::GetNumComponentsInType(compSize);
    }
}

// TODO: refactor this template signature to get rid of typename
// AttributeInserter
template <typename VecType, typename BufferType, typename AttributeInserter>
void FillAttributeVector(const tinygltf::Primitive& primitive,
                         const tinygltf::Model& model,
                         const std::string& attrName, int compSize,
                         size_t vertexCount,
                         std::vector<VecType>& attributeVector,
                         const AttributeInserter& attributeInserter) {
    int byteStride = 0;
    const BufferType* buffer = nullptr;

    FillVertexAttribute(primitive, model, attrName, compSize, &buffer,
                        byteStride);

    if (buffer != nullptr) {
        attributeVector.reserve(vertexCount);

        for (size_t v = 0; v < vertexCount; ++v) {
            attributeVector.push_back(
                attributeInserter(&buffer[v * byteStride]));
        }
    }
}

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

void FillPrimitiveVertices(Primitive& resultPrimitive,
                           const tinygltf::Primitive& primitive,
                           const tinygltf::Model& model) {
    // Position attribute is required
    AssertD(primitive.attributes.find("POSITION") !=
            primitive.attributes.end());

    const tinygltf::Accessor& posAccessor =
        model.accessors[primitive.attributes.find("POSITION")->second];
    const size_t vertexCount = posAccessor.count;

    FillAttributeVector<Position, float>(
        primitive, model, "POSITION", TINYGLTF_TYPE_VEC3, vertexCount,
        resultPrimitive.positions, [](const float* bufferPtr) -> Position {
            return Position(glm::make_vec3(bufferPtr), 1.0f);
        });

    FillAttributeVector<Normal, float>(
        primitive, model, "NORMAL", TINYGLTF_TYPE_VEC3, vertexCount,
        resultPrimitive.normals, [](const float* bufferPtr) -> Normal {
            return glm::make_vec2(bufferPtr);
        });

    FillAttributeVector<Uv, float>(
        primitive, model, "TEXCOORD_0", TINYGLTF_TYPE_VEC2, vertexCount,
        resultPrimitive.uvs,
        [](const float* bufferPtr) -> Uv { return glm::make_vec2(bufferPtr); });

    FillAttributeVector<Uv, float>(
        primitive, model, "TEXCOORD_1", TINYGLTF_TYPE_VEC2, vertexCount,
        resultPrimitive.uvs1,
        [](const float* bufferPtr) -> Uv { return glm::make_vec2(bufferPtr); });

    glm::vec3 posMin =
        glm::vec3(posAccessor.minValues[0], posAccessor.minValues[1],
                  posAccessor.minValues[2]);
    glm::vec3 posMax =
        glm::vec3(posAccessor.maxValues[0], posAccessor.maxValues[1],
                  posAccessor.maxValues[2]);

    resultPrimitive.bbox = AABBox(posMin, posMax);
}

void FillPrimitiveIndices(Primitive& resultPrimitive,
                          const tinygltf::Primitive& primitive,
                          const tinygltf::Model& model) {
    std::vector<uint32_t> indexBuffer;
    indexBuffer.reserve(4096);

    const tinygltf::Accessor& accessor =
        model.accessors[primitive.indices > -1 ? primitive.indices : 0];
    const tinygltf::BufferView& bufferView =
        model.bufferViews[accessor.bufferView];
    const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

    const size_t indexCount = accessor.count;
    const void* dataPtr =
        &(buffer.data[accessor.byteOffset + bufferView.byteOffset]);

    switch (accessor.componentType) {
        case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT: {
            const uint32_t* buf = static_cast<const uint32_t*>(dataPtr);
            for (size_t index = 0; index < indexCount; index++) {
                indexBuffer.push_back(buf[index]);
            }
            break;
        }
        case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT: {
            const uint16_t* buf = static_cast<const uint16_t*>(dataPtr);
            for (size_t index = 0; index < indexCount; index++) {
                indexBuffer.push_back(buf[index]);
            }
            break;
        }
        case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE: {
            const uint8_t* buf = static_cast<const uint8_t*>(dataPtr);
            for (size_t index = 0; index < indexCount; index++) {
                indexBuffer.push_back(buf[index]);
            }
            break;
        }
        default:
            throw Exception(
                "Failed to load asset! Index component type %i not supported!",
                accessor.componentType);
    }

    resultPrimitive.indices = std::move(indexBuffer);
}

Primitive CreatePrimitive(const tinygltf::Primitive& primitive,
                          const tinygltf::Model& model) {
    Primitive resultPrimitive;
    resultPrimitive.material = std::make_shared<Material>();

    bool hasIndices = primitive.indices > -1;

    FillPrimitiveVertices(resultPrimitive, primitive, model);

    if (hasIndices) {
        FillPrimitiveIndices(resultPrimitive, primitive, model);
    }

    return resultPrimitive;
}

void CreateRenderComponent(entt::registry& registry, entt::entity& entity,
                           const tinygltf::Node& node,
                           const tinygltf::Model& model) {
    // Node contains mesh data
    if (node.mesh > -1) {
        const tinygltf::Mesh mesh = model.meshes[node.mesh];

        for (size_t j = 0; j < mesh.primitives.size(); j++) {
            const tinygltf::Primitive& primitive = mesh.primitives[j];

            auto primitiveNode = registry.create();
            auto& renderComponent = registry.emplace<RenderComponent>(entity);

            renderComponent.primitive = CreatePrimitive(primitive, model);

            Entity::AddChild(registry, entity, primitiveNode);
        }
    }
}

void LoadNode(entt::registry& registry, entt::entity& parent,
              const tinygltf::Node& node, const tinygltf::Model& model) {
    auto entityNode = registry.create();

    CreateTransformComponent(registry, entityNode, node);
    CreateRenderComponent(registry, entityNode, node, model);

    // Node with children
    if (node.children.size() > 0) {
        for (size_t i = 0; i < node.children.size(); i++) {
            LoadNode(registry, entityNode, model.nodes[node.children[i]],
                     model);
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
    scene.GetRegistry().emplace<TransformComponent>(rootNode);

    const tinygltf::Scene& gltfScene =
        model.scenes[model.defaultScene > -1 ? model.defaultScene : 0];
    for (size_t i = 0; i < gltfScene.nodes.size(); i++) {
        const tinygltf::Node node = model.nodes[gltfScene.nodes[i]];
        SLoadGltfModelToScene::LoadNode(scene.GetRegistry(), rootNode, node,
                                        model);
    }

    Entity::AddChild(scene.GetRegistry(), scene.GetRootEntity(), rootNode);

    LogI(eTag::kAssetLoading)
        << "glTF model loaded successfully: " << modelFilePath << std::endl;
}