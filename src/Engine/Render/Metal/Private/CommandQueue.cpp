#include "Engine/Render/Metal/CommandQueue.hpp"

#include "Engine/Common/Assert.hpp"
#include "Engine/Render/Metal/LogicalDevice.hpp"
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

MTL::CommandQueue& SHV::Metal::CommandQueue::GetCommandQueue() const {
    AssertD(commandQueue != nullptr);
    return *commandQueue;
}