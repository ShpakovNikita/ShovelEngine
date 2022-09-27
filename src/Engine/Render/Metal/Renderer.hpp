#pragma once

#include <memory>

#include "Engine/Render/Renderer.hpp"

namespace MTL {
class Device;
class Buffer;
}  // namespace MTL

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

    // TODO: remove;
    void LoadPrimitives();
    void UnloadPrimitives();

   private:
    std::unique_ptr<LogicalDevice> device;
    std::unique_ptr<RenderPipeline> renderPipeline;
    std::unique_ptr<CommandQueue> commandQueue;

    std::shared_ptr<RenderBatch> renderBatch;

    Window& window;
};
}  // namespace Metal

}  // namespace SHV