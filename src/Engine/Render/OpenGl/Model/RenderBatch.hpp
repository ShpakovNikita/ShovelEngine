#pragma once

#include <GL/glew.h>

#include "Engine/Render/Model/RenderBatch.hpp"

#include <cstddef>
#include <memory>

namespace SHV {
struct Primitive;
namespace OpenGl {
class LogicalDevice;

class RenderBatch : public SHV::RenderBatch {
   public:
    ~RenderBatch();
    RenderBatch();

    static RenderBatch Create(void* data, size_t vertexCount,
                              size_t vertexLayoutSize);

    static RenderBatch Create(const Primitive& primitive);

    inline size_t GetVertexCount() const { return vertexCount; }
    inline size_t GetVertexLayoutSize() const { return vertexLayoutSize; }

    void Bind() const;
    void Unbind() const;

    void Release() const;

   private:
    GLuint vertexBufferObject;
    GLuint vertexArrayObject;

    size_t vertexCount;
    size_t vertexLayoutSize;
};
}  // namespace OpenGl
}  // namespace SHV