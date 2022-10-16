#pragma once

#include <cstddef>

namespace MTL {
class Device;
}

namespace SHV {
namespace Metal {
class WindowContext;

class LogicalDevice {
   public:
    LogicalDevice();

    void SetUp();
    void TearDown();

    void AssignDeviceToWindow(WindowContext& metalWindow);
    void RemoveDeviceFromWindow(WindowContext& metalWindow);

    MTL::Device& GetDevice() const;

   private:
    MTL::Device* device = nullptr;
};
}  // namespace Metal
}  // namespace SHV
