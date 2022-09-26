#pragma once

#include <simd/simd.h>

namespace SHV::Metal {

struct BasicVertexLayout {
    simd::float4 pos;
    simd::float2 normal;
    simd::float2 uv;
};

}  // namespace SHV::Metal