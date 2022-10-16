#pragma once

namespace SHV {
enum ProfilerSystems : uint64_t {
    Physics = 1 << 0,
    Rendering = 1 << 1,
    ECS = 1 << 2
};

constexpr uint64_t kActiveProfilerSystems = ProfilerSystems::Physics |
                                            ProfilerSystems::Rendering |
                                            ProfilerSystems::ECS;
}  // namespace SHV