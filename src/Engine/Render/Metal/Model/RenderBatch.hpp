#pragma once

#include <cstddef>
#include <memory>

#include "Engine/Render/Model/RenderBatch.hpp"

namespace MTL {
class Buffer;
}

namespace SHV {
struct Primitive;
namespace Metal {
class LogicalDevice;

class RenderBatch : public SHV::RenderBatch {
   public:
    virtual ~RenderBatch();

    static std::shared_ptr<RenderBatch> Create(LogicalDevice& device,
                                               void* data, size_t vertexCount,
                                               size_t vertexLayoutSize);

    static std::shared_ptr<RenderBatch> Create(LogicalDevice& device,
                                               const Primitive& primitive);

    inline size_t GetVertexCount() const { return vertexCount; }
    inline size_t GetVertexLayoutSize() const { return vertexLayoutSize; }

    MTL::Buffer& GetVertexBuffer() const;

   private:
    RenderBatch();

    MTL::Buffer* vertexBuffer = nullptr;

    size_t vertexCount;
    size_t vertexLayoutSize;
};
}  // namespace Metal
}  // namespace SHV