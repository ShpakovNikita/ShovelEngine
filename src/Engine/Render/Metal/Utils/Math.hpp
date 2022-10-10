#pragma once

#include <simd/simd.h>
#include <glm/glm.hpp>

namespace SHV::Metal {
simd::float4 GlmToSimdFloat4(const glm::vec4& vec);
simd::float4x4 GlmToSimdMatrix(const glm::mat4& mat);
}  // namespace SHV::Metal