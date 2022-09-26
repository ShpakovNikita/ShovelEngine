#pragma once

#include <memory>

namespace MTL {
class Device;
struct Buffer;
}  // namespace MTL

namespace SHV {

namespace Metal {

class Window;
class LogicalDevice;
class RenderPipeline;
class CommandQueue;

class Renderer {
   public:
    Renderer(Window& metalWindow);
    ~Renderer();

    void SetUp();
    void TearDown();

    void Draw();

   private:
    std::unique_ptr<LogicalDevice> device;
    std::unique_ptr<RenderPipeline> renderPipeline;
    std::unique_ptr<CommandQueue> commandQueue;
    MTL::Buffer* vertexBuffer = nullptr;

    Window& window;
};
}  // namespace Metal

}  // namespace SHV