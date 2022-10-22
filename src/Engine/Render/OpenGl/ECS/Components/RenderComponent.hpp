#pragma once

#include <memory>

#include "Engine/Render/OpenGl/Model/RenderBatch.hpp"
#include "Engine/Render/OpenGl/Model/RenderMaterial.hpp"

namespace SHV::OpenGl {
struct RenderComponent {
    // TODO: shared ptr
    RenderBatch renderBatch;
    std::shared_ptr<RenderMaterial> renderMaterial;
};
}  // namespace SHV::OpenGl