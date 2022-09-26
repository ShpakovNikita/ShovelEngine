#include "Engine/Render/Metal/CommandQueue.hpp"

#include "Engine/Render/Metal/LogicalDevice.hpp"
#include "Engine/Utils/Assert.hpp"
#include "Metal/Metal.hpp"

SHV::Metal::CommandQueue::CommandQueue(LogicalDevice& aDevice)
    : device(aDevice) {}

void SHV::Metal::CommandQueue::SetUp() {
    commandQueue = device.GetDevice().newCommandQueue();
}

void SHV::Metal::CommandQueue::TearDown() {
    AssertD(commandQueue != nullptr);
    commandQueue->release();
    commandQueue = nullptr;
}
