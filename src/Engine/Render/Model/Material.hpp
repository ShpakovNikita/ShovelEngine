#pragma once

namespace SHV {

enum eShader { kBasicShader };

struct Material {
    eShader materialShader = eShader::kBasicShader;
};

}  // namespace SHV