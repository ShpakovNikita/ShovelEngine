#pragma once

#include "Texture.hpp"

#include <map>
#include <memory>

namespace SHV {

enum eShader { kBasicShader, kPbrShader };

struct Material {
    eShader materialShader = eShader::kBasicShader;

    std::map<std::string, std::shared_ptr<Texture>> textures;
};

}  // namespace SHV