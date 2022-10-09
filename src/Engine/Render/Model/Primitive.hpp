#pragma once

#include <vector>

#include "VertexLayout.hpp"
#include "AABBox.hpp"

namespace SHV {
struct Material;

struct Primitive {
    Primitive();
    Primitive(std::shared_ptr<Material> material);

    std::vector<Uv> uvs;
    std::vector<Uv> uvs1;  // Additional UV Layout
    std::vector<Position> positions;
    std::vector<Normal> normals;

    std::vector<size_t> indices;

    AABBox bbox;

    std::shared_ptr<Material> material;
};
}  // namespace SHV