#pragma once

#include <vector>

#include "VertexLayout.hpp"

namespace SHV {
struct Material;

struct Primitive {
    Primitive(const Material& material);

    std::vector<Uv> uvs;
    std::vector<Position> positions;
    std::vector<Normal> normals;

    const Material& material;
};
}  // namespace SHV