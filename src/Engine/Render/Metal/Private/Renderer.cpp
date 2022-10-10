#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include "Engine/Render/Metal/Renderer.hpp"

#include "Engine/Common/Assert.hpp"
#include "Engine/Common/Logger.hpp"
#include "Engine/Render/Metal/CommandQueue.hpp"
#include "Engine/Render/Metal/LogicalDevice.hpp"
#include "Engine/Render/Metal/Model/RenderBatch.hpp"
#include "Engine/Render/Metal/RenderPipeline.hpp"
#include "Engine/Render/Metal/Window.hpp"
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

SHV::Metal::Renderer::Renderer(Window& metalWindow)
    : SHV::Renderer(), window(metalWindow){};
SHV::Metal::Renderer::~Renderer() { AssertD(device == nullptr); }

void SHV::Metal::Renderer::SetUp() {
    LogD(eTag::kMetalAPI) << "Setting up Metal Renderer" << std::endl;

    device = std::make_unique<SHV::Metal::LogicalDevice>();
    device->SetUp();
    device->AssignDeviceToWindow(window);

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
    AssertD(drawPool == nullptr);
    drawPool = NS::AutoreleasePool::alloc()->init();

    AssertD(surface == nullptr);
    surface = window.NextDrawable();

    MTL::ClearColor clear_color(window.GetWindowConfig().clearColor.r,
                                window.GetWindowConfig().clearColor.g,
                                window.GetWindowConfig().clearColor.b,
                                window.GetWindowConfig().clearColor.a);
    renderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
    auto attachment = renderPassDescriptor->colorAttachments()->object(0);
    attachment->setClearColor(clear_color);
    attachment->setLoadAction(MTL::LoadActionClear);
    attachment->setTexture(surface->texture());

    commandBuffer = commandQueue->GetCommandQueue().commandBuffer();
    renderCommandEncoder =
        commandBuffer->renderCommandEncoder(renderPassDescriptor);
}

void SHV::Metal::Renderer::EndFrame() {
    renderCommandEncoder->endEncoding();

    AssertD(surface != nullptr);
    commandBuffer->presentDrawable(surface);
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
