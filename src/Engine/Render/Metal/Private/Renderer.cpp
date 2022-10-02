#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include "Engine/Render/Metal/Renderer.hpp"

#include "Engine/Common/Assert.hpp"
#include "Engine/Render/Metal/CommandQueue.hpp"
#include "Engine/Render/Metal/LogicalDevice.hpp"
#include "Engine/Render/Metal/Model/RenderBatch.hpp"
#include "Engine/Render/Metal/RenderPipeline.hpp"
#include "Engine/Render/Metal/Window.hpp"
#include "Engine/Render/Model/Material.hpp"
#include "Engine/Render/Model/Primitive.hpp"
#include "Metal/Metal.hpp"
#include "QuartzCore/QuartzCore.hpp"

SHV::Metal::Renderer::Renderer(Window& metalWindow)
    : SHV::Renderer(), window(metalWindow){};
SHV::Metal::Renderer::~Renderer() { AssertD(device == nullptr); }

void SHV::Metal::Renderer::SetUp() {
    device = std::make_unique<SHV::Metal::LogicalDevice>();
    device->SetUp();
    device->AssignDeviceToWindow(window);

    renderPipeline = std::make_unique<SHV::Metal::RenderPipeline>(
        *device, "basic_vertex", "basic_fragment");
    renderPipeline->SetUp();

    commandQueue = std::make_unique<SHV::Metal::CommandQueue>(*device);
    commandQueue->SetUp();

    // TODO: remove
    LoadPrimitives();
}

void SHV::Metal::Renderer::LoadPrimitives() {
    Material material = {SHV::eShader::kBasicShader};
    Primitive primitive = {material};

    primitive.positions = {{0, 1, 0, 1}, {-1, -1, 0, 1}, {1, -1, 0, 1}};
    primitive.normals = {{0, 1.0}, {0, 1.0}, {0, 1.0}};
    primitive.uvs = {{0, 0}, {0, 0}, {0, 0}};

    renderBatch = SHV::Metal::RenderBatch::Create(*device, primitive);
}

void SHV::Metal::Renderer::UnloadPrimitives() {
    AssertD(renderBatch.use_count() == 1);
    renderBatch = nullptr;
}

void SHV::Metal::Renderer::TearDown() {
    // TODO: remove
    UnloadPrimitives();

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
    NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();

    CA::MetalDrawable* surface = window.NextDrawable();

    MTL::ClearColor clear_color(window.GetWindowConfig().clearColor.r,
                                window.GetWindowConfig().clearColor.g,
                                window.GetWindowConfig().clearColor.b,
                                window.GetWindowConfig().clearColor.a);
    auto renderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
    auto attachment = renderPassDescriptor->colorAttachments()->object(0);
    attachment->setClearColor(clear_color);
    attachment->setLoadAction(MTL::LoadActionClear);
    attachment->setTexture(surface->texture());

    auto buffer = commandQueue->GetCommandQueue().commandBuffer();
    auto encoder = buffer->renderCommandEncoder(renderPassDescriptor);
    encoder->setRenderPipelineState(&renderPipeline->GetRenderPipelineState());
    encoder->setVertexBuffer(&renderBatch->GetVertexBuffer(), 0, 0);
    encoder->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, 0,
                            renderBatch->GetVertexCount(), 1);
    encoder->endEncoding();

    buffer->presentDrawable(surface);
    buffer->commit();

    pool->release();
}