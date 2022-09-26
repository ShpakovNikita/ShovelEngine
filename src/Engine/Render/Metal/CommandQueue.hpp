#pragma once

namespace MTL {
class CommandQueue;
}

namespace SHV {
namespace Metal {
class LogicalDevice;

class CommandQueue {
   public:
    CommandQueue(LogicalDevice& device);

    void SetUp();
    void TearDown();

   private:
    LogicalDevice& device;

    MTL::CommandQueue* commandQueue;
};
}  // namespace Metal
}  // namespace SHV
