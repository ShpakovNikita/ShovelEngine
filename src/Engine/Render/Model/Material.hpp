#pragma once

#include "Texture.hpp"

#include <memory>

namespace SHV {

enum eShader { kBasicShader, kPbrShader };

struct Material {
    eShader materialShader = eShader::kBasicShader;

    std::shared_ptr<Texture> texture = nullptr;
};

}  // namespace SHV