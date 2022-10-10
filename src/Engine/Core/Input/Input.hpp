#pragma once

#include <array>

#include "InputKeys.hpp"

namespace SHV {
struct Input {
    std::array<bool, static_cast<size_t>(eKey::kSize)> pressedKeys{};

    inline bool IsKeyPressed(eKey key) const {
        return pressedKeys[static_cast<size_t>(key)];
    }
};
}  // namespace SHV