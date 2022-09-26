#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include "Engine/Render/Metal/Renderer.hpp"

#include <simd/simd.h>

#include <array>

#include "Engine/Render/Metal/LogicalDevice.hpp"
#include "Engine/Render/Metal/RenderPipeline.hpp"
#include "Engine/Render/Metal/Window.hpp"
#include "Engine/Utils/Assert.hpp"
#include "Metal/Metal.hpp"

SHV::Metal::Renderer::Renderer(Window& metalWindow)
    : device(nullptr), window(metalWindow){};
SHV::Metal::Renderer::~Renderer() { AssertD(device == nullptr); }

void SHV::Metal::Renderer::SetUp() {
    device = std::make_unique<SHV::Metal::LogicalDevice>();
    device->SetUp();

    device->AssignDeviceToWindow(window);

    const std::array<simd::float3, 9> vertexData = {0.0, 1.0, 0.0,  -1.0, -1.0,
                                                    0.0, 1.0, -1.0, 0.0};

    const int dataSize = vertexData.size() * sizeof(vertexData[0]);
    vertexBuffer = device->GetDevice().newBuffer(
        vertexData.data(), dataSize, MTL::ResourceStorageModeManaged);

    renderPipeline = std::make_unique<SHV::Metal::RenderPipeline>(
        *device, "basic_vertex", "basic_fragment");
    renderPipeline->SetUp();
}

void SHV::Metal::Renderer::TearDown() {
    AssertD(renderPipeline != nullptr);
    renderPipeline->TearDown();
    renderPipeline = nullptr;

    AssertD(device != nullptr);
    device->TearDown();
    device = nullptr;
}

void SHV::Metal::Renderer::Draw() {}