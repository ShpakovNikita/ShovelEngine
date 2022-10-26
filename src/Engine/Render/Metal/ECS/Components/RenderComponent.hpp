#pragma once

#include <memory>

#include "Engine/Render/Metal/Model/RenderBatch.hpp"
#include "Engine/Render/Metal/Model/RenderMaterial.hpp"

namespace SHV::Metal {
struct RenderComponent {
    RenderBatch renderBatch;
    std::shared_ptr<RenderMaterial> renderMaterial;
};
}  // namespace SHV::Metal