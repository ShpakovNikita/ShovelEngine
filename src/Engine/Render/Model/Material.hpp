#pragma once

#include "Texture.hpp"
#include "MaterialShader.hpp"

#include <map>
#include <memory>
#include <any>

namespace SHV {

using MaterialParam = std::any;

enum class eAlphaMode { kNone, kMask, kBlend };

struct Material {
    eMaterialShader materialShader = eMaterialShader::kBasicShader;

    std::map<std::string, std::shared_ptr<Texture>> textures;

    // TODO: Unused now
    std::map<std::string, MaterialParam> params;

    // TODO: Unused now
    eAlphaMode alphaMode;
    float alphaCutoff = 0.5f;
};

}  // namespace SHV