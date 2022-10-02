#pragma once

#include <string>

#include "Renderer.hpp"
#include "glm/vec4.hpp"

namespace SHV {
struct WindowConfig {
    const uint32_t width;
    const uint32_t height;

    std::string name;

    eRenderApi renderApi;

    glm::vec4 clearColor = {0.2f, 0.3f, 0.3f, 1.0f};
};
}  // namespace SHV