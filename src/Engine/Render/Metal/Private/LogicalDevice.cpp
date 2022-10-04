//
// Created by Mikita on 26.09.22.
//

#include "Engine/Render/Metal/LogicalDevice.hpp"

#include "Engine/Common/Assert.hpp"
#include "Engine/Common/Logger.hpp"
#include "Engine/Render/Metal/MetalBridge.hpp"
#include "Engine/Render/Metal/Window.hpp"
#include "Metal/Metal.hpp"

using namespace SHV;

Metal::LogicalDevice::LogicalDevice() {}

void Metal::LogicalDevice::SetUp() {
    LogD(eTag::kMetalAPI) << "Setting up Logical Device" << std::endl;

    device = ::MTL::CreateSystemDefaultDevice();
};

void Metal::LogicalDevice::TearDown() {
    device->release();
    device = nullptr;

    LogD(eTag::kMetalAPI) << "Logical Device teared down" << std::endl;
};

void Metal::LogicalDevice::AssignDeviceToWindow(Window& metalWindow) {
    AssignMetalDevice(metalWindow.GetCAMetalLayer(), device);
};

MTL::Device& Metal::LogicalDevice::GetDevice() const {
    AssertD(device != nullptr);

    return *device;
};