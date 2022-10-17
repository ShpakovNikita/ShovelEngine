#pragma once

namespace SHV {
enum ProfilerSystems : uint64_t {
    Physics = 1 << 0,
    Rendering = 1 << 1,
    ECS = 1 << 2
};

// Modify this value if you need to remove or add some subsystems to collected
// profiler data
constexpr uint64_t kActiveProfilerSystems = ProfilerSystems::Physics |
                                            ProfilerSystems::Rendering |
                                            ProfilerSystems::ECS;
}  // namespace SHV