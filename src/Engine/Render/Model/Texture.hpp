#pragma once

#include <string>

namespace SHV {
struct Texture {
    std::string texturePath;
    uint32_t width, height;
    uint32_t channels;

    // TODO: remove from this struct?
    void* data = nullptr;
};
}  // namespace SHV