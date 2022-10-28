#include "Engine/Tools/Scene/GltfSceneLoader.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "tiny_gltf.h"

#include "Engine/Render/Model/Material.hpp"
#include "Engine/Render/Model/ShaderParamNames.hpp"
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
    bool hasIndices = primitive.indices > -1;

    FillPrimitiveVertices(resultPrimitive, primitive, model);

    if (hasIndices) {
        FillPrimitiveIndices(resultPrimitive, primitive, model);
    }

    return resultPrimitive;
}

// from GLTF2 specs
TextureSampler::eAddressMode GetAddressMode(int32_t wrapMode) {
    switch (wrapMode) {
        case 10497:
            return TextureSampler::eAddressMode::kRepeat;
        case 33071:
            return TextureSampler::eAddressMode::kClampToEdge;
        case 33648:
            return TextureSampler::eAddressMode::kClampToBorder;
        default:
            return TextureSampler::eAddressMode::kRepeat;
    }
}

// from GLTF2 specs
TextureSampler::eFilter GetFilter(int32_t filterMode) {
    switch (filterMode) {
        case 9728:
            return TextureSampler::eFilter::kNearest;
        case 9729:
            return TextureSampler::eFilter::kLinear;
        case 9984:
            return TextureSampler::eFilter::kNearest;
        case 9985:
            return TextureSampler::eFilter::kNearest;
        case 9986:
            return TextureSampler::eFilter::kLinear;
        case 9987:
            return TextureSampler::eFilter::kLinear;
        default:
            return TextureSampler::eFilter::kNearest;
    }
}

void LoadTextureSamplers(const tinygltf::Model& input,
                         std::vector<TextureSampler>& textureSamplers) {
    for (tinygltf::Sampler smpl : input.samplers) {
        TextureSampler sampler;
        sampler.minFilter = GetFilter(smpl.minFilter);
        sampler.magFilter = GetFilter(smpl.magFilter);
        sampler.addressModeU = GetAddressMode(smpl.wrapS);
        sampler.addressModeV = GetAddressMode(smpl.wrapT);
        sampler.addressModeW = sampler.addressModeV;
        textureSamplers.push_back(sampler);
    }
}

void LoadTextures(const tinygltf::Model& input,
                  std::vector<std::shared_ptr<Texture>>& textures,
                  const std::vector<TextureSampler>& textureSamplers) {
    textures.reserve(input.textures.size());

    textures.reserve(input.textures.size());

    for (const tinygltf::Texture& tex : input.textures) {
        const tinygltf::Image& image [[maybe_unused]] =
            input.images[tex.source];
        TextureSampler textureSampler;
        if (tex.sampler != -1) {
            textureSampler = textureSamplers[tex.sampler];
        }
        std::shared_ptr<Texture> texture = std::make_shared<Texture>(
            &image.image[0], image.width, image.height, image.component,
            image.component == 3 ? eTextureFormat::kRGB8
                                 : eTextureFormat::kRGBA8,
            textureSampler);
        textures.push_back(texture);
    }
}

void LoadMaterials(const tinygltf::Model& input,
                   std::vector<Material>& materials,
                   const std::vector<std::shared_ptr<Texture>>& textures) {
    materials.reserve(input.materials.size());

    for (const tinygltf::Material& mat : input.materials) {
        Material material;
        material.materialShader = eMaterialShader::kPbrShader;

        if (mat.values.find("baseColorTexture") != mat.values.end()) {
            material.textures[SHV::PbrParams::kAlbedoMap] =
                textures[mat.values.at("baseColorTexture").TextureIndex()];
            /* TODO: get texture uv location, for now just uv0
            material->texCoordSets.baseColor =
                mat.values.at("baseColorTexture").TextureTexCoord();
                */
        }
        if (mat.values.find("metallicRoughnessTexture") != mat.values.end()) {
            material.textures[SHV::PbrParams::kMetallicRoughnessMap] =
                textures[mat.values.at("metallicRoughnessTexture")
                             .TextureIndex()];
        }
        if (mat.values.find("roughnessFactor") != mat.values.end()) {
            material.params[SHV::PbrParams::kRoughnessFactor] =
                static_cast<float>(mat.values.at("roughnessFactor").Factor());
        }
        if (mat.values.find("metallicFactor") != mat.values.end()) {
            material.params[SHV::PbrParams::kMetallicFactor] =
                static_cast<float>(mat.values.at("metallicFactor").Factor());
        }
        if (mat.values.find("baseColorFactor") != mat.values.end()) {
            material.params[SHV::PbrParams::kBaseColorFactor] = glm::make_vec4(
                mat.values.at("baseColorFactor").ColorFactor().data());
        }
        if (mat.additionalValues.find("normalTexture") !=
            mat.additionalValues.end()) {
            material.textures[SHV::PbrParams::kNormalMap] =
                textures[mat.additionalValues.at("normalTexture")
                             .TextureIndex()];
        }
        if (mat.additionalValues.find("emissiveTexture") !=
            mat.additionalValues.end()) {
            material.textures[SHV::PbrParams::kEmissiveMap] =
                textures[mat.additionalValues.at("emissiveTexture")
                             .TextureIndex()];
        }
        if (mat.additionalValues.find("occlusionTexture") !=
            mat.additionalValues.end()) {
            material.textures[SHV::PbrParams::kAoMap] =
                textures[mat.additionalValues.at("occlusionTexture")
                             .TextureIndex()];
        }
        if (mat.additionalValues.find("alphaMode") !=
            mat.additionalValues.end()) {
            tinygltf::Parameter param = mat.additionalValues.at("alphaMode");
            if (param.string_value == "BLEND") {
                material.alphaMode = eAlphaMode::kBlend;
            }
            if (param.string_value == "MASK") {
                material.alphaCutoff = 0.5f;
                material.alphaMode = eAlphaMode::kMask;
            }
        }
        if (mat.additionalValues.find("alphaCutoff") !=
            mat.additionalValues.end()) {
            material.alphaCutoff = static_cast<float>(
                mat.additionalValues.at("alphaCutoff").Factor());
        }
        if (mat.additionalValues.find("emissiveFactor") !=
            mat.additionalValues.end()) {
            material.params[SHV::PbrParams::kEmissiveFactor] = glm::vec4(
                glm::make_vec3(mat.additionalValues.at("emissiveFactor")
                                   .ColorFactor()
                                   .data()),
                1.0);
        }
        materials.push_back(std::move(material));
    }
}

void CreateRenderComponent(entt::registry& registry, entt::entity& entity,
                           const tinygltf::Node& node,
                           const tinygltf::Model& model,
                           const std::vector<Material>& materials) {
    // Node contains mesh data
    if (node.mesh > -1) {
        const tinygltf::Mesh mesh = model.meshes[node.mesh];

        for (size_t j = 0; j < mesh.primitives.size(); j++) {
            const tinygltf::Primitive& primitive = mesh.primitives[j];

            auto primitiveNode = registry.create();
            auto& renderComponent =
                registry.emplace<RenderComponent>(primitiveNode);
            
            renderComponent.primitive = CreatePrimitive(primitive, model);
            renderComponent.material = materials[primitive.material];

            Entity::AddChild(registry, entity, primitiveNode);
        }
    }
}

void LoadNode(entt::registry& registry, entt::entity& parent,
              const tinygltf::Node& node, const tinygltf::Model& model,
              const std::vector<Material>& materials) {
    auto entityNode = registry.create();

    CreateTransformComponent(registry, entityNode, node);
    CreateRenderComponent(registry, entityNode, node, model, materials);

    // Node with children
    if (node.children.size() > 0) {
        for (size_t i = 0; i < node.children.size(); i++) {
            LoadNode(registry, entityNode, model.nodes[node.children[i]], model,
                     materials);
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

    std::vector<TextureSampler> textureSamplers;
    std::vector<std::shared_ptr<Texture>> textures;
    std::vector<Material> materials;

    SLoadGltfModelToScene::LoadTextureSamplers(model, textureSamplers);
    SLoadGltfModelToScene::LoadTextures(model, textures, textureSamplers);
    SLoadGltfModelToScene::LoadMaterials(model, materials, textures);

    for (size_t i = 0; i < gltfScene.nodes.size(); i++) {
        const tinygltf::Node node = model.nodes[gltfScene.nodes[i]];
        SLoadGltfModelToScene::LoadNode(scene.GetRegistry(), rootNode, node,
                                        model, materials);
    }

    Entity::AddChild(scene.GetRegistry(), scene.GetRootEntity(), rootNode);

    LogI(eTag::kAssetLoading)
        << "glTF model loaded successfully: " << modelFilePath << std::endl;
}