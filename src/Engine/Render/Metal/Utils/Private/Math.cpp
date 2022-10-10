#include "Engine/Render/Metal/Utils/Math.hpp"

using namespace SHV::Metal;

simd::float4 SHV::Metal::GlmToSimdFloat4(const glm::vec4& vec) {
    return simd_make_float4(vec.x, vec.y, vec.z, vec.w);
}

simd::float4x4 SHV::Metal::GlmToSimdMatrix(const glm::mat4& mat) {
    return simd_matrix(GlmToSimdFloat4(mat[0]), GlmToSimdFloat4(mat[1]),
                       GlmToSimdFloat4(mat[2]), GlmToSimdFloat4(mat[3]));
}