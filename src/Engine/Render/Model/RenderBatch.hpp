#pragma once

#include <cstddef>
#include <memory>

namespace SHV {
struct Primitive;

enum eBatchType { kVertexOnly, kIndices };

class RenderBatch {
   public:
    RenderBatch();
    virtual ~RenderBatch();

    inline size_t GetVertexCount() const { return vertexCount; }
    inline size_t GetVertexLayoutSize() const { return vertexLayoutSize; }

   private:
    size_t vertexCount;
    size_t vertexLayoutSize;
};
}  // namespace SHV