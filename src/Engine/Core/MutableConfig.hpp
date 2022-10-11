#pragma once

#include "Engine/Render/Renderer.hpp"

namespace SHV {
struct MutableConfig {
    float fpsLimit;

    eRenderApi renderApi;
};
}  // namespace SHV