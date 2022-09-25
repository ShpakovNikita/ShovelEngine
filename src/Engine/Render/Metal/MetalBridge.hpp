#pragma once

namespace CA {
class MetalDrawable;
}

namespace MTL {
class Device;
}

namespace SHV::Metal {
void AssignMetalDevice(void* layer, MTL::Device* device);
CA::MetalDrawable* NextDrawable(void* layer);
}  // namespace SHV::Metal