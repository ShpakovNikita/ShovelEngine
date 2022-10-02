#include "Engine/Render/Metal/Model/RenderBatch.hpp"

#include <vector>

#include "Engine/Common/Assert.hpp"
#include "Engine/Render/Metal/LogicalDevice.hpp"
#include "Engine/Render/Model/Material.hpp"
#include "Engine/Render/Model/Primitive.hpp"
#include "Metal/Metal.hpp"

using namespace SHV;

Metal::RenderBatch::RenderBatch() = default;

Metal::RenderBatch::~RenderBatch() {
    AssertD(vertexBuffer != nullptr);
    vertexBuffer->release();
    vertexBuffer = nullptr;
}

std::shared_ptr<Metal::RenderBatch> Metal::RenderBatch::Create(
    Metal::LogicalDevice& device, void* data, size_t vertexCount,
    size_t vertexLayoutSize) {
    auto renderBatch =
        std::shared_ptr<Metal::RenderBatch>(new Metal::RenderBatch());

    const size_t dataSize = vertexCount * vertexLayoutSize;
    renderBatch->vertexBuffer = device.GetDevice().newBuffer(
        data, dataSize, MTL::ResourceStorageModeManaged);

    renderBatch->vertexCount = vertexCount;
    renderBatch->vertexLayoutSize = vertexLayoutSize;

    return renderBatch;
}

std::shared_ptr<Metal::RenderBatch> Metal::RenderBatch::Create(
    Metal::LogicalDevice& device, const Primitive& primitive) {
    // TODO: some type smart refactor
    switch (primitive.material.materialShader) {
        case kBasicShader:
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

            return Metal::RenderBatch::Create(device, primitiveData.data(),
                                              vertexCount,
                                              sizeof(BasicVertexLayout));
    }
}

MTL::Buffer& Metal::RenderBatch::GetVertexBuffer() const {
    AssertD(vertexBuffer != nullptr);
    return *vertexBuffer;
}
