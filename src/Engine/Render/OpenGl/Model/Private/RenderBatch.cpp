#include "Engine/Render/OpenGl//Model/RenderBatch.hpp"

#include <vector>
#include <numeric>

#include "Engine/Common/Assert.hpp"
#include "Engine/Render/Model/Material.hpp"
#include "Engine/Render/Model/Primitive.hpp"

#include <Tracy.hpp>
#include "Engine/Common/ProfilerSystems.hpp"

using namespace SHV;

OpenGl::RenderBatch::RenderBatch() = default;

OpenGl::RenderBatch::~RenderBatch() = default;

OpenGl::RenderBatch OpenGl::RenderBatch::Create(const void* data,
                                                size_t vertexCount,
                                                size_t vertexLayoutSize,
                                                const uint32_t* indices,
                                                size_t indexCount) {
    OpenGl::RenderBatch renderBatch;

    glGenBuffers(1, &renderBatch.vertexBufferObject);
    glGenBuffers(1, &renderBatch.elementBufferObject);
    glGenVertexArrays(1, &renderBatch.vertexArrayObject);

    glBindVertexArray(renderBatch.vertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, renderBatch.vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexLayoutSize, data,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderBatch.elementBufferObject);

    if (indexCount > 0) {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint32_t),
                     indices, GL_STATIC_DRAW);
        renderBatch.indexCount = indexCount;
    } else {
        AssertD(vertexCount % 3 == 0);

        std::vector<uint32_t> generatedIndices(vertexCount);
        std::iota(generatedIndices.begin(), generatedIndices.end(), 0);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     generatedIndices.size() * sizeof(uint32_t), indices,
                     GL_STATIC_DRAW);
        renderBatch.indexCount = generatedIndices.size();
    }

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

    renderBatch.vertexCount = vertexCount;
    renderBatch.vertexLayoutSize = vertexLayoutSize;

    return renderBatch;
}

OpenGl::RenderBatch OpenGl::RenderBatch::Create(const Primitive& primitive) {
    ZoneNamedN(
        __tracy, "OpenGl RenderBatch Create",
        static_cast<bool>(kActiveProfilerSystems & ProfilerSystems::Rendering));

    AssertD(primitive.material != nullptr);

    // TODO: some type smart refactor
    switch (primitive.material->materialShader) {
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
                primitiveData.data(), vertexCount, sizeof(BasicVertexLayout),
                primitive.indices.data(), primitive.indices.size());
    }
}

void OpenGl::RenderBatch::Bind() const {
    AssertD(vertexArrayObject != 0);
    glBindVertexArray(vertexArrayObject);
}

void OpenGl::RenderBatch::Unbind() const {
    AssertD(vertexArrayObject != 0);
    glBindVertexArray(0);
}

void OpenGl::RenderBatch::Release() const {
    AssertD(elementBufferObject != 0);
    glDeleteBuffers(1, &elementBufferObject);

    AssertD(vertexBufferObject != 0);
    glDeleteBuffers(1, &vertexBufferObject);

    AssertD(vertexArrayObject != 0);
    glDeleteVertexArrays(1, &vertexArrayObject);
}