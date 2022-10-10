#pragma once

#include <simd/simd.h>

namespace SHV::Metal {

struct BasicVertexLayout {
    simd::float4 pos;
    simd::float2 normal;
    simd::float2 uv;
};

struct UniformsData {
    simd::float4x4 projection = {1.0f};
    simd::float4x4 view = {1.0f};
    simd::float4x4 model = {1.0f};
};

}  // namespace SHV::Metal