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
#include "Engine/Render/Metal/RenderPipeline.hpp"
#include "Engine/Render/Metal/RenderPipelineCache.hpp"
#include "Engine/Render/Metal/WindowContext.hpp"
#include "Engine/Render/Metal/Shaders/ShaderDefinitions.h"

#include "Engine/ECS/Scene.hpp"
#include "Engine/ECS/Entity.hpp"
#include "Engine/ECS/Components/CameraComponent.hpp"
#include "Engine/Render/Metal/ECS/RenderBatcher.hpp"
#include "Engine/Render/Metal/Model/GPUTexture.hpp"
#include "Engine/Render/Metal/ECS/Components/RenderComponent.hpp"
#include "Engine/Render/ECS/Systems/RenderSystem.hpp"
#include "Engine/ECS/Components/RelationshipComponent.hpp"
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

    commandQueue = std::make_unique<SHV::Metal::CommandQueue>(*device);
    commandQueue->SetUp();

    depthTexture = CreateDepthTexture();
}

void SHV::Metal::Renderer::TearDown() {
    AssertD(depthTexture != nullptr);
    depthTexture->release();
    depthTexture = nullptr;

    AssertD(commandQueue != nullptr);
    commandQueue->TearDown();
    commandQueue = nullptr;

    AssertD(device != nullptr);
    device->RemoveDeviceFromWindow(windowContext);
    device->TearDown();
    device = nullptr;

    LogD(eTag::kMetalAPI) << "Metal Renderer teared down" << std::endl;
}

void SHV::Metal::Renderer::SetUpScene(Scene& scene) {
    std::unique_ptr<SHV::RenderBatcher> renderBatcher =
        std::make_unique<SHV::Metal::RenderBatcher>(*device, *commandQueue);
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

    const auto renderView =
        scene.GetRegistry()
            .view<SHV::RenderComponent, SHV::RelationshipComponent>();

    auto cameraEntity = scene.GetEntityWithActiveCamera();
    AssertE(cameraEntity != entt::null);

    const auto& cameraComponent =
        scene.GetRegistry().get<SHV::CameraComponent>(cameraEntity);
    const auto& cameraTransform =
        scene.GetRegistry().get<SHV::TransformComponent>(cameraEntity);

    for (const auto& [entity, renderComponent, relationshipComponent] :
         renderView.each()) {
        // TODO: optimize
        if (!Entity::IsNodesConnected(scene.GetRegistry(),
                                      scene.GetRootEntity(), entity)) {
            continue;
        }

        const auto& metalRenderComponent =
            scene.GetRegistry().try_get<SHV::Metal::RenderComponent>(entity);
        AssertE(metalRenderComponent != nullptr &&
                metalRenderComponent->renderMaterial != nullptr);

        RenderPipeline& renderPipeline =
            metalRenderComponent->renderMaterial->GetRenderPipeline();

        const TransformComponent* transformComponent =
            Entity::GetFirstComponentInHierarchy<TransformComponent>(
                scene.GetRegistry(), entity);
        AssertE(transformComponent != nullptr);

        UniformsData uniformsData;
        uniformsData.projection = GlmToSimdMatrix(cameraComponent.projection);
        uniformsData.model =
            GlmToSimdMatrix(transformComponent->GetWorldMatrix());
        uniformsData.view =
            GlmToSimdMatrix(cameraComponent.GetViewMatrix(cameraTransform));

        renderCommandEncoder->setVertexBytes(&uniformsData,
                                             sizeof(uniformsData), 1);

        const auto& renderBatch = metalRenderComponent->renderBatch;

        renderCommandEncoder->setRenderPipelineState(
            &renderPipeline.GetRenderPipelineState());

        metalRenderComponent->renderMaterial->Bind(*renderCommandEncoder);

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

    {
        ZoneNamedN(__tracy_scope_2, "Next Drawable",
                   static_cast<bool>(kActiveProfilerSystems &
                                     ProfilerSystems::Rendering));
        AssertD(surface == nullptr);
        surface = windowContext.NextDrawable();
    }

    MTL::ClearColor clearColor(
        windowContext.GetWindow().GetWindowConfig().clearColor.r,
        windowContext.GetWindow().GetWindowConfig().clearColor.g,
        windowContext.GetWindow().GetWindowConfig().clearColor.b,
        windowContext.GetWindow().GetWindowConfig().clearColor.a);
    renderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();

    auto depthAttachment = renderPassDescriptor->depthAttachment();
    depthAttachment->setTexture(depthTexture);
    depthAttachment->setClearDepth(1.0f);
    depthAttachment->setStoreAction(MTL::StoreAction::StoreActionDontCare);

    auto colorAttachment = renderPassDescriptor->colorAttachments()->object(0);
    colorAttachment->setClearColor(clearColor);
    colorAttachment->setLoadAction(MTL::LoadActionClear);
    colorAttachment->setTexture(surface->texture());

    {
        ZoneNamedN(__tracy_scope_2, "New Command Buffer",
                   static_cast<bool>(kActiveProfilerSystems &
                                     ProfilerSystems::Rendering));
        commandBuffer = commandQueue->GetCommandQueue().commandBuffer();
        commandBuffer->addCompletedHandler(
            [this](MTL::CommandBuffer* /*buffer*/) {
                frameBufferingSemaphore.release();
            });
    }

    MTL::DepthStencilDescriptor* depthDescriptor =
        MTL::DepthStencilDescriptor::alloc()->init();
    depthDescriptor->setDepthCompareFunction(
        MTL::CompareFunction::CompareFunctionLessEqual);
    depthDescriptor->setDepthWriteEnabled(true);

    MTL::DepthStencilState* depthStencilState =
        device->GetDevice().newDepthStencilState(depthDescriptor);

    renderCommandEncoder =
        commandBuffer->renderCommandEncoder(renderPassDescriptor);
    renderCommandEncoder->setDepthStencilState(depthStencilState);
}

void SHV::Metal::Renderer::WaitForFrameExecutionFinish() {
    ZoneNamedN(
        __tracy, "Metal Render WaitForFrameExecutionFinish",
        static_cast<bool>(kActiveProfilerSystems & ProfilerSystems::Rendering));

    frameBufferingSemaphore.acquire();
}

void SHV::Metal::Renderer::EndFrame() {
    ZoneNamedN(
        __tracy, "Metal Render EndFrame",
        static_cast<bool>(kActiveProfilerSystems & ProfilerSystems::Rendering));

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

MTL::Texture* SHV::Metal::Renderer::CreateDepthTexture() {
    const size_t width = windowContext.GetWindow().GetWindowSize().x;
    const size_t height = windowContext.GetWindow().GetWindowSize().y;

    auto textureDescr = MTL::TextureDescriptor::texture2DDescriptor(
        MTL::PixelFormat::PixelFormatDepth32Float_Stencil8, width, height,
        false);
    textureDescr->setStorageMode(MTL::StorageMode::StorageModePrivate);
    textureDescr->setUsage(MTL::TextureUsageRenderTarget);

    return device->GetDevice().newTexture(textureDescr);
}
