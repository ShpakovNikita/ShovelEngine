#include "Engine/Render/OpenGl//Model/RenderBatch.hpp"

#include <vector>

#include "Engine/Common/Assert.hpp"
#include "Engine/Render/Model/Material.hpp"
#include "Engine/Render/Model/Primitive.hpp"

using namespace SHV;

OpenGl::RenderBatch::RenderBatch() = default;

OpenGl::RenderBatch::~RenderBatch() {
    AssertD(vertexBufferObject != 0);
    glDeleteBuffers(1, &vertexBufferObject);

    AssertD(vertexArrayObject != 0);
    glDeleteVertexArrays(1, &vertexArrayObject);
}

std::shared_ptr<OpenGl::RenderBatch> OpenGl::RenderBatch::Create(
    void* data, size_t vertexCount, size_t vertexLayoutSize) {
    auto renderBatch =
        std::shared_ptr<OpenGl::RenderBatch>(new OpenGl::RenderBatch());

    // const size_t dataSize = vertexCount * vertexLayoutSize;

    glGenBuffers(1, &renderBatch->vertexBufferObject);
    glGenVertexArrays(1, &renderBatch->vertexArrayObject);

    glBindVertexArray(renderBatch->vertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, renderBatch->vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexLayoutSize, data,
                 GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, vertexLayoutSize, (void*)0);
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertexLayoutSize,
                          (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // uv attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexLayoutSize,
                          (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    renderBatch->vertexCount = vertexCount;
    renderBatch->vertexLayoutSize = vertexLayoutSize;

    return renderBatch;
}

std::shared_ptr<OpenGl::RenderBatch> OpenGl::RenderBatch::Create(
    const Primitive& primitive) {
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

            return OpenGl::RenderBatch::Create(
                primitiveData.data(), vertexCount, sizeof(BasicVertexLayout));
    }
}

void OpenGl::RenderBatch::Bind() {
    AssertD(vertexArrayObject != 0);
    glBindVertexArray(vertexArrayObject);
}
