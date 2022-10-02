#pragma once

#include <memory>

#include "Engine/Render/Renderer.hpp"

namespace MTL {
class Device;
class Buffer;
class RenderPassDescriptor;
class CommandBuffer;
class RenderCommandEncoder;
}  // namespace MTL

namespace NS {
class AutoreleasePool;
}  // namespace NS

namespace SHV {

namespace Metal {

class Window;
class LogicalDevice;
class RenderPipeline;
class CommandQueue;
class RenderBatch;

class Renderer : public ::SHV::Renderer {
   public:
    Renderer(Window& metalWindow);
    virtual ~Renderer();

    virtual void SetUp();
    virtual void TearDown();

    virtual void Draw();

    virtual void BeginFrame();
    virtual void EndFrame();

    LogicalDevice& GetLogicalDevice() const;

    MTL::RenderPassDescriptor& GetRenderPassDescriptor() const;
    MTL::CommandBuffer& GetCommandBuffer() const;
    MTL::RenderCommandEncoder& GetRenderCommandEncoder() const;

   private:
    // TODO: remove;
    void LoadPrimitives();
    void UnloadPrimitives();

    std::unique_ptr<LogicalDevice> device;
    std::unique_ptr<RenderPipeline> renderPipeline;
    std::unique_ptr<CommandQueue> commandQueue;

    std::shared_ptr<RenderBatch> renderBatch;

    NS::AutoreleasePool* drawPool = nullptr;
    MTL::RenderPassDescriptor* renderPassDescriptor = nullptr;
    MTL::CommandBuffer* commandBuffer = nullptr;
    MTL::RenderCommandEncoder* renderCommandEncoder = nullptr;

    Window& window;
};
}  // namespace Metal

}  // namespace SHV