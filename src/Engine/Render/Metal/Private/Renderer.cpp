#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include "Engine/Render/Metal/Renderer.hpp"

#include <simd/simd.h>

#include <array>

#include "Engine/Render/Metal/CommandQueue.hpp"
#include "Engine/Render/Metal/LogicalDevice.hpp"
#include "Engine/Render/Metal/RenderPipeline.hpp"
#include "Engine/Render/Metal/Window.hpp"
#include "Engine/Utils/Assert.hpp"
#include "Metal/Metal.hpp"
#include "QuartzCore/QuartzCore.hpp"

SHV::Metal::Renderer::Renderer(Window& metalWindow) : window(metalWindow){};
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

    commandQueue = std::make_unique<SHV::Metal::CommandQueue>(*device);
    commandQueue->SetUp();
}

void SHV::Metal::Renderer::TearDown() {
    AssertD(commandQueue != nullptr);
    commandQueue->TearDown();
    commandQueue = nullptr;

    AssertD(renderPipeline != nullptr);
    renderPipeline->TearDown();
    renderPipeline = nullptr;

    AssertD(device != nullptr);
    device->TearDown();
    device = nullptr;
}

void SHV::Metal::Renderer::Draw() {
    CA::MetalDrawable* surface = window.NextDrawable();

    MTL::ClearColor clear_color(152.0 / 255.0, 23.0 / 255.0, 42.0 / 255.0, 1.0);
    auto pass_descriptor = MTL::RenderPassDescriptor::alloc()->init();
    auto attachment = pass_descriptor->colorAttachments()->object(0);
    attachment->setClearColor(clear_color);
    attachment->setLoadAction(MTL::LoadActionClear);
    attachment->setTexture(surface->texture());

    auto buffer = commandQueue->GetCommandQueue().commandBuffer();
    auto encoder = buffer->renderCommandEncoder(pass_descriptor);
    encoder->endEncoding();
    buffer->presentDrawable(surface);
    buffer->commit();
}