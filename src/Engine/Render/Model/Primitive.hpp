#pragma once

#include <vector>

#include "VertexLayout.hpp"

namespace SHV {
struct Material;

struct Primitive {
    Primitive();
    Primitive(std::shared_ptr<Material> material);

    std::vector<Uv> uvs;
    std::vector<Position> positions;
    std::vector<Normal> normals;

    std::vector<size_t> indices;

    std::shared_ptr<Material> material;
};
}  // namespace SHV