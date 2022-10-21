#pragma once

#include "Engine/Render/Model/Primitive.hpp"
#include "Engine/Render/Model/Material.hpp"

namespace SHV {
struct RenderComponent {
    Primitive primitive;
    Material material;
};
}  // namespace SHV
