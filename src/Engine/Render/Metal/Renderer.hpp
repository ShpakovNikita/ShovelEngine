#pragma once

#include <memory>

#include "Engine/Render/Renderer.hpp"

#include <semaphore>
#include <thread>

namespace MTL {
class Device;
class Buffer;
class RenderPassDescriptor;
class CommandBuffer;
class RenderCommandEncoder;
class Texture;
}  // namespace MTL

namespace NS {
class AutoreleasePool;
}  // namespace NS

namespace CA {
class MetalDrawable;
}  // namespace CA

namespace SHV {

class Texture;

namespace Metal {

class WindowContext;
class LogicalDevice;
class RenderPipeline;
class CommandQueue;
class RenderBatch;

class Renderer : public ::SHV::Renderer {
   public:
    Renderer(WindowContext& metalWindowContext);
    virtual ~Renderer();

    virtual void SetUp();
    virtual void TearDown();

    virtual void Draw(const Scene& scene);
    virtual std::shared_ptr<Texture> Draw(const Scene& scene, const Texture& renderTargetPrototype);

    virtual void SetUpScene(Scene& scene) const;
    virtual void TearDownScene(Scene& scene) const;

    virtual void BeginFrame();
    virtual void EndFrame();

    LogicalDevice& GetLogicalDevice() const;

    MTL::RenderPassDescriptor& GetRenderPassDescriptor() const;
    MTL::CommandBuffer& GetCommandBuffer() const;
    MTL::RenderCommandEncoder& GetRenderCommandEncoder() const;

    virtual void WaitForFrameExecutionFinish();

   private:
    void BeginFrame(MTL::Texture* renderPassTarget, MTL::Texture* depthTexture);
    void EndFrame(bool offscreen);

    MTL::Texture* CreateDepthTexture(size_t width, size_t height);

    std::unique_ptr<LogicalDevice> device;
    std::unique_ptr<CommandQueue> commandQueue;

    NS::AutoreleasePool* drawPool = nullptr;
    MTL::RenderPassDescriptor* renderPassDescriptor = nullptr;
    MTL::CommandBuffer* commandBuffer = nullptr;
    MTL::RenderCommandEncoder* renderCommandEncoder = nullptr;
    CA::MetalDrawable* surface = nullptr;
    MTL::Texture* depthTexture = nullptr;

    WindowContext& windowContext;

    std::counting_semaphore<1> frameBufferingSemaphore{0};
};
}  // namespace Metal

}  // namespace SHV