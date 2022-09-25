#pragma once

namespace MTL {
class Device;
}  // namespace MTL

namespace SHV {

namespace Metal {

class Window;

class Renderer {
   public:
    Renderer(Window& metalWindow);

    void SetUp();
    void TearDown();

   private:
    MTL::Device* device = nullptr;
    
    Window& window;
};
}  // namespace Metal

}  // namespace SHV