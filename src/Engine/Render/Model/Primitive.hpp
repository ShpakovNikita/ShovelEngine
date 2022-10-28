#pragma once

#include <vector>

#include "VertexLayout.hpp"
#include "AABBox.hpp"

namespace SHV {
struct Primitive {
    Primitive();

    std::vector<Uv> uvs;
    std::vector<Uv> uvs1;  // Additional UV Layout
    std::vector<Position> positions;
    std::vector<Normal> normals;

    std::vector<uint32_t> indices;

    AABBox bbox;
};
}  // namespace SHV