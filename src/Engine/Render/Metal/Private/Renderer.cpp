#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include "Engine/Render/Metal/Renderer.hpp"

#include "Engine/Render/Metal/MetalBridge.hpp"
#include "Engine/Render/Metal/Window.hpp"
#include "Metal/Metal.hpp"

SHV::Metal::Renderer::Renderer(Window& metalWindow) : window(metalWindow){};

void SHV::Metal::Renderer::SetUp() {
    device = ::MTL::CreateSystemDefaultDevice();

    AssignMetalDevice(window.GetCAMetalLayer(), device);
}

void SHV::Metal::Renderer::TearDown() { device->release(); }
