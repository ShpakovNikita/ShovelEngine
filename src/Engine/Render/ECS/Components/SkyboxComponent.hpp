#pragma once

#include "Engine/Render/Model/Primitive.hpp"
#include "Engine/Render/Model/Material.hpp"
#include "Engine/Render/Model/Texture.hpp"

#include <memory>

namespace SHV {
struct RenderComponent;

struct SkyboxComponent {
    std::shared_ptr<Texture> equirectangularTexture;

    bool isVisible = true;
    // TODO: remove after all debug, this should be the only behaviour
    bool convertToCubeMaps = false;
};
}  // namespace SHV
