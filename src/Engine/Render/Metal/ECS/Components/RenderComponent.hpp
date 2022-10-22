#pragma once

#include "Engine/Render/Metal/Model/RenderBatch.hpp"
#include "Engine/Render/Metal/Model/RenderMaterial.hpp"

namespace SHV::Metal {
struct RenderComponent {
    RenderBatch renderBatch;
    RenderMaterial renderMaterial;
};
}  // namespace SHV::Metal