#pragma once

#include <cstddef>

namespace MTL {
class Device;
}

namespace SHV {
namespace Metal {
class Window;

class LogicalDevice {
   public:
    LogicalDevice();

    void SetUp();
    void TearDown();

    void AssignDeviceToWindow(Window& metalWindow);

    MTL::Device& GetDevice() const;

   private:
    MTL::Device* device = nullptr;
};
}  // namespace Metal
}  // namespace SHV
