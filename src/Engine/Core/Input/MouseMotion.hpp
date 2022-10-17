#pragma once

#include <cstdint>

namespace SHV {
struct MouseMotion {
    int32_t x = {0}, y = {0};    // Relative position of mouse
    int32_t dx = {0}, dy = {0};  // Relative motion of mouse on current frame
};
}  // namespace SHV