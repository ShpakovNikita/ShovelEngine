#pragma once

#include <cstdint>
#include <limits>

namespace SHV {
using RenderBatchHandle = std::uint32_t;
constexpr RenderBatchHandle kInvalidBatchHandle =
    std::numeric_limits<RenderBatchHandle>::max();
}  // namespace SHV