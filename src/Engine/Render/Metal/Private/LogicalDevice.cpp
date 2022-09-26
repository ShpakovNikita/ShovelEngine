//
// Created by Mikita on 26.09.22.
//

#include "Engine/Render/Metal/LogicalDevice.hpp"

#include "Engine/Render/Metal/MetalBridge.hpp"
#include "Engine/Render/Metal/Window.hpp"
#include "Engine/Utils/Assert.hpp"
#include "Metal/Metal.hpp"

using namespace SHV;

Metal::LogicalDevice::LogicalDevice() {}

void Metal::LogicalDevice::SetUp() {
    device = ::MTL::CreateSystemDefaultDevice();
};

void Metal::LogicalDevice::TearDown() {
    device->release();
    device = nullptr;
};

void Metal::LogicalDevice::AssignDeviceToWindow(Window& metalWindow) {
    AssignMetalDevice(metalWindow.GetCAMetalLayer(), device);
};

MTL::Device& Metal::LogicalDevice::GetDevice() const {
    AssertD(device != nullptr);

    return *device;
};