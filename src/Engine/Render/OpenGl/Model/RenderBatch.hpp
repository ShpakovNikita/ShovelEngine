#pragma once

#include <GL/glew.h>

#include "Engine/Render/Model/RenderBatch.hpp"
#include "Engine/Render/Model/MaterialShader.hpp"

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

    static RenderBatch Create(const Primitive& primitive,
                              const eMaterialShader shaderLayout);

    inline size_t GetVertexCount() const { return vertexCount; }
    inline size_t GetIndexCount() const { return indexCount; }
    inline size_t GetVertexLayoutSize() const { return vertexLayoutSize; }

    void Bind() const;
    void Unbind() const;

    void Release() const;

   private:
    static RenderBatch Create(const void* data, size_t vertexCount,
                              size_t vertexLayoutSize, const uint32_t* indices,
                              size_t indexCount);

    GLuint vertexBufferObject;
    GLuint vertexArrayObject;
    GLuint elementBufferObject;

    size_t vertexCount;
    size_t indexCount;
    size_t vertexLayoutSize;
};
}  // namespace OpenGl
}  // namespace SHV