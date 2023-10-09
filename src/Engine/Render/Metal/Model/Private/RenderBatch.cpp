#include "Engine/Render/Metal/Model/RenderBatch.hpp"

#include <vector>
#include <numeric>

#include "Engine/Common/Assert.hpp"
#include "Engine/Render/Metal/LogicalDevice.hpp"
#include "Engine/Render/Model/Material.hpp"
#include "Engine/Render/Model/Primitive.hpp"
#include "Metal/Metal.hpp"

#include <Tracy.hpp>
#include "Engine/Common/ProfilerSystems.hpp"

using namespace SHV;

Metal::RenderBatch::RenderBatch() = default;

Metal::RenderBatch::~RenderBatch() = default;

Metal::RenderBatch Metal::RenderBatch::Create(Metal::LogicalDevice& device,
                                              void* data, size_t vertexCount,
                                              size_t vertexLayoutSize,
                                              const uint32_t* indices,
                                              size_t indexCount) {
    Metal::RenderBatch renderBatch;

    renderBatch.vertexBuffer = device.GetDevice().newBuffer(
        data, vertexCount * vertexLayoutSize, MTL::ResourceStorageModeManaged);

    renderBatch.vertexCount = vertexCount;
    renderBatch.vertexLayoutSize = vertexLayoutSize;

    if (indexCount > 0) {
        renderBatch.indexBuffer =
            device.GetDevice().newBuffer(indices, indexCount * sizeof(uint32_t),
                                         MTL::ResourceStorageModeManaged);
        renderBatch.indexCount = indexCount;
    } else {
        AssertD(vertexCount % 3 == 0);

        std::vector<uint32_t> generatedIndices(vertexCount);
        std::iota(generatedIndices.begin(), generatedIndices.end(), 0);

        renderBatch.indexBuffer = device.GetDevice().newBuffer(
            indices, generatedIndices.size() * sizeof(uint32_t),
            MTL::ResourceStorageModeManaged);
        renderBatch.indexCount = generatedIndices.size();
    }

    return renderBatch;
}

Metal::RenderBatch Metal::RenderBatch::Create(
    Metal::LogicalDevice& device, const Primitive& primitive,
    const eMaterialShader shaderLayout) {
    ZoneNamedN(
        __tracy, "Metal RenderBatch Create",
        static_cast<bool>(kActiveProfilerSystems & ProfilerSystems::Rendering));

    // TODO: some type smart refactor
    switch (shaderLayout) {
        case kPbrShader:
        case kBasicShader:
        {
            const size_t vertexCount = primitive.positions.size();
            std::vector<BasicVertexLayout> primitiveData;
            primitiveData.reserve(vertexCount);
            
            AssertD(primitive.uvs.size() == vertexCount);
            AssertD(primitive.normals.size() == vertexCount);
            AssertD(primitive.positions.size() == vertexCount);
            
            for (size_t i = 0; i < vertexCount; ++i) {
                primitiveData.push_back({primitive.positions[i],
                    primitive.normals[i],
                    primitive.uvs[i]});
            }
            
            return Metal::RenderBatch::Create(
                                              device, primitiveData.data(), vertexCount,
                                              sizeof(BasicVertexLayout), primitive.indices.data(),
                                              primitive.indices.size());
        }
        case kSkyboxShader:
        case kSkyboxEquirectangularShader:
        {
            const size_t vertexCount = primitive.positions.size();
            std::vector<SkyboxVertexLayout> primitiveData;
            primitiveData.reserve(vertexCount);

            AssertD(primitive.positions.size() == vertexCount);
            
            for (size_t i = 0; i < vertexCount; ++i) {
                primitiveData.push_back({primitive.positions[i]});
            }
            
            return Metal::RenderBatch::Create(
                                              device, primitiveData.data(), vertexCount,
                                              sizeof(SkyboxVertexLayout), primitive.indices.data(),
                                              primitive.indices.size());
        }
    }
}

void Metal::RenderBatch::Release() {
    AssertD(indexBuffer != nullptr);
    indexBuffer->release();
    indexBuffer = nullptr;
}

MTL::Buffer& Metal::RenderBatch::GetIndexBuffer() const {
    AssertD(indexBuffer != nullptr);
    return *indexBuffer;
}

MTL::Buffer& Metal::RenderBatch::GetVertexBuffer() const {
    AssertD(vertexBuffer != nullptr);
    return *vertexBuffer;
}
