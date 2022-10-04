#pragma once

#include <string>
#include <vector>

#include "Engine/Render/Renderer.hpp"

namespace SHV {
struct ImmutableConfig {
    const uint32_t width;
    const uint32_t height;

    eRenderApi renderApi;

    std::vector<std::string> args;

    std::string windowName = "Shovel Engine";
};
}  // namespace SHV
