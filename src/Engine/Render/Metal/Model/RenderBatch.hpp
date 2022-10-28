#pragma once

#include <cstddef>
#include <memory>

#include "Engine/Render/Model/RenderBatch.hpp"
#include "Engine/Render/Model/MaterialShader.hpp"

namespace MTL {
class Buffer;
}

namespace SHV {
struct Primitive;
namespace Metal {
class LogicalDevice;

class RenderBatch : public SHV::RenderBatch {
   public:
    RenderBatch();
    virtual ~RenderBatch();

    static RenderBatch Create(LogicalDevice& device, const Primitive& primitive,
                              const eMaterialShader shaderLayout);

    inline size_t GetVertexCount() const { return vertexCount; }
    inline size_t GetIndexCount() const { return indexCount; }
    inline size_t GetVertexLayoutSize() const { return vertexLayoutSize; }

    MTL::Buffer& GetIndexBuffer() const;
    MTL::Buffer& GetVertexBuffer() const;

    void Release();

   private:
    static RenderBatch Create(LogicalDevice& device, void* data,
                              size_t vertexCount, size_t vertexLayoutSize,
                              const uint32_t* indices, size_t indexCount);

    MTL::Buffer* indexBuffer = nullptr;
    MTL::Buffer* vertexBuffer = nullptr;

    size_t vertexCount;
    size_t indexCount;
    size_t vertexLayoutSize;
};
}  // namespace Metal
}  // namespace SHV