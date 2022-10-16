#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include "Engine/Render/Metal/Renderer.hpp"

#include <Tracy.hpp>

#include "Engine/Core/Window.hpp"

#include "Engine/Common/Assert.hpp"
#include "Engine/Common/ProfilerSystems.hpp"
#include "Engine/Common/Logger.hpp"

#include "Engine/Render/Metal/CommandQueue.hpp"
#include "Engine/Render/Metal/LogicalDevice.hpp"
#include "Engine/Render/Metal/Model/RenderBatch.hpp"
#include "Engine/Render/Metal/RenderPipeline.hpp"
#include "Engine/Render/Metal/WindowContext.hpp"
#include "Engine/Render/Metal/Shaders/ShaderDefinitions.h"
#include "Engine/Render/Model/Material.hpp"

#include "Engine/ECS/Scene.hpp"
#include "Engine/ECS/Entity.hpp"
#include "Engine/ECS/Components/CameraComponent.hpp"
#include "Engine/Render/Metal/ECS/RenderBatcher.hpp"
#include "Engine/Render/Metal/ECS/Components/RenderComponent.hpp"
#include "Engine/Render/ECS/Systems/RenderSystem.hpp"
#include "Engine/Render/Metal/Utils/Math.hpp"

#include "Metal/Metal.hpp"
#include "QuartzCore/QuartzCore.hpp"

SHV::Metal::Renderer::Renderer(WindowContext& aMetalWindowContext)
    : SHV::Renderer(), windowContext(aMetalWindowContext){};
SHV::Metal::Renderer::~Renderer() { AssertD(device == nullptr); }

void SHV::Metal::Renderer::SetUp() {
    LogD(eTag::kMetalAPI) << "Setting up Metal Renderer" << std::endl;

    device = std::make_unique<SHV::Metal::LogicalDevice>();
    device->SetUp();
    device->AssignDeviceToWindow(windowContext);

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
    device->RemoveDeviceFromWindow(windowContext);
    device->TearDown();
    device = nullptr;

    LogD(eTag::kMetalAPI) << "Metal Renderer teared down" << std::endl;
}

void SHV::Metal::Renderer::SetUpScene(Scene& scene) {
    std::unique_ptr<SHV::RenderBatcher> renderBatcher =
        std::make_unique<SHV::Metal::RenderBatcher>(*device);
    scene.AddSystem<SHV::RenderSystem<SHV::Metal::RenderComponent>>(
        std::move(renderBatcher));
}

void SHV::Metal::Renderer::TearDownScene(Scene& scene) {
    scene.RemoveSystem<SHV::RenderSystem<SHV::Metal::RenderComponent>>();
}

void SHV::Metal::Renderer::Draw(const Scene& scene) {
    ZoneNamedN(
        __tracy, "Metal Render Draw",
        static_cast<bool>(kActiveProfilerSystems & ProfilerSystems::Rendering));

    const auto renderView = scene.GetRegistry().view<SHV::RenderComponent>();

    auto cameraEntity = scene.GetEntityWithActiveCamera();
    AssertE(cameraEntity != entt::null);

    const auto& cameraComponent =
        scene.GetRegistry().get<SHV::CameraComponent>(cameraEntity);
    const auto& cameraTransform =
        scene.GetRegistry().get<SHV::TransformComponent>(cameraEntity);

    for (const auto& [entity, renderComponent] : renderView.each()) {
        const auto& metalRenderComponent =
            scene.GetRegistry().try_get<SHV::Metal::RenderComponent>(entity);
        AssertE(metalRenderComponent != nullptr);

        const TransformComponent* transformComponent =
            Entity::GetFirstComponentInHierarchy<TransformComponent>(
                scene.GetRegistry(), entity);
        AssertE(transformComponent != nullptr);

        UniformsData uniformsData;
        uniformsData.projection = GlmToSimdMatrix(cameraComponent.projection);
        uniformsData.model =
            GlmToSimdMatrix(transformComponent->GetLocalMatrix());
        uniformsData.view = GlmToSimdMatrix(cameraTransform.GetLocalMatrix());

        renderCommandEncoder->setVertexBytes(&uniformsData,
                                             sizeof(uniformsData), 1);

        const auto& renderBatch = metalRenderComponent->renderBatch;

        renderCommandEncoder->setRenderPipelineState(
            &renderPipeline->GetRenderPipelineState());

        renderCommandEncoder->setVertexBuffer(&renderBatch.GetVertexBuffer(), 0,
                                              0);

        renderCommandEncoder->drawIndexedPrimitives(
            MTL::PrimitiveType::PrimitiveTypeTriangle,
            renderBatch.GetIndexCount(), MTL::IndexType::IndexTypeUInt32,
            &renderBatch.GetIndexBuffer(), 0, 1);
    }
}

void SHV::Metal::Renderer::BeginFrame() {
    ZoneNamedN(
        __tracy, "Metal Render BeginFrame",
        static_cast<bool>(kActiveProfilerSystems & ProfilerSystems::Rendering));

    AssertD(drawPool == nullptr);
    drawPool = NS::AutoreleasePool::alloc()->init();

    AssertD(surface == nullptr);
    surface = windowContext.NextDrawable();

    MTL::ClearColor clear_color(
        windowContext.GetWindow().GetWindowConfig().clearColor.r,
        windowContext.GetWindow().GetWindowConfig().clearColor.g,
        windowContext.GetWindow().GetWindowConfig().clearColor.b,
        windowContext.GetWindow().GetWindowConfig().clearColor.a);
    renderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
    auto attachment = renderPassDescriptor->colorAttachments()->object(0);
    attachment->setClearColor(clear_color);
    attachment->setLoadAction(MTL::LoadActionClear);
    attachment->setTexture(surface->texture());

    commandBuffer = commandQueue->GetCommandQueue().commandBuffer();
    renderCommandEncoder =
        commandBuffer->renderCommandEncoder(renderPassDescriptor);
}

void SHV::Metal::Renderer::WaitForFrameExecutionFinish() {
    ZoneNamedN(
        __tracy, "Metal Render WaitForFrameExecutionFinish",
        static_cast<bool>(kActiveProfilerSystems & ProfilerSystems::Rendering));

    commandBuffer->waitUntilCompleted();
    /*
    std::unique_lock<std::mutex> lk(frameExecutionMutex);
    frameExecutionCV.wait(lk,
                          [this]() -> bool { return frameExecutionFinished; });
                          */
}

void SHV::Metal::Renderer::EndFrame() {
    ZoneNamedN(
        __tracy, "Metal Render EndFrame",
        static_cast<bool>(kActiveProfilerSystems & ProfilerSystems::Rendering));

    renderCommandEncoder->endEncoding();

    AssertD(surface != nullptr);
    commandBuffer->presentDrawable(surface);

    /*
    frameExecutionFinished = false;
    commandBuffer->addCompletedHandler(
        [this](MTL::CommandBuffer*) {
            // Notify main thread
            frameExecutionFinished = true;
            frameExecutionCV.notify_one();
        });*/

    commandBuffer->commit();
    surface = nullptr;

    AssertD(drawPool != nullptr);
    drawPool->release();
    drawPool = nullptr;
}

SHV::Metal::LogicalDevice& SHV::Metal::Renderer::GetLogicalDevice() const {
    return *device;
}

MTL::RenderPassDescriptor& SHV::Metal::Renderer::GetRenderPassDescriptor()
    const {
    return *renderPassDescriptor;
}

MTL::CommandBuffer& SHV::Metal::Renderer::GetCommandBuffer() const {
    return *commandBuffer;
}

MTL::RenderCommandEncoder& SHV::Metal::Renderer::GetRenderCommandEncoder()
    const {
    return *renderCommandEncoder;
}
