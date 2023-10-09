#include "Engine/Render/Metal/MetalBridge.hpp"

#include "Metal/Metal.hpp"
#include "Metal/Metal.h"
#include "QuartzCore/QuartzCore.hpp"
#include "QuartzCore/QuartzCore.h"

using namespace SHV;

void Metal::AssignMetalDevice(void* layer, MTL::Device* device)
{
    CAMetalLayer* metalLayer = (CAMetalLayer*) layer;
    metalLayer.device = (__bridge id<MTLDevice>)(device);
}

void Metal::RemoveMetalDevice(void* layer)
{
    CAMetalLayer* metalLayer = (CAMetalLayer*) layer;
    metalLayer.device = nullptr;
}

CA::MetalDrawable* Metal::NextDrawable(void* layer) {
  CAMetalLayer* metalLayer = (CAMetalLayer*) layer;
  metalLayer.pixelFormat = MTLPixelFormatRGBA8Unorm;
  id <CAMetalDrawable> metalDrawable = [metalLayer nextDrawable];
  CA::MetalDrawable* pMetalCppDrawable = ( __bridge CA::MetalDrawable*) metalDrawable;
  return pMetalCppDrawable;
}
