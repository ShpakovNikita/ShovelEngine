#pragma once

#include <GL/glew.h>

#include <cstddef>
#include <memory>

namespace SHV {
struct Primitive;
namespace OpenGl {
class LogicalDevice;

class RenderBatch {
   public:
    ~RenderBatch();

    static std::shared_ptr<RenderBatch> Create(void* data, size_t vertexCount,
                                               size_t vertexLayoutSize);

    static std::shared_ptr<RenderBatch> Create(const Primitive& primitive);

    inline size_t GetVertexCount() const { return vertexCount; }
    inline size_t GetVertexLayoutSize() const { return vertexLayoutSize; }

    void Bind();

   private:
    RenderBatch();

    GLuint vertexBufferObject;
    GLuint vertexArrayObject;

    size_t vertexCount;
    size_t vertexLayoutSize;
};
}  // namespace OpenGl
}  // namespace SHV