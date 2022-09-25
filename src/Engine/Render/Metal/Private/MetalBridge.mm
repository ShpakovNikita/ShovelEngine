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

CA::MetalDrawable* Metal::NextDrawable(void* layer) {
  CAMetalLayer* metalLayer = (CAMetalLayer*) layer;
  id <CAMetalDrawable> metalDrawable = [metalLayer nextDrawable];
  CA::MetalDrawable* pMetalCppDrawable = ( __bridge CA::MetalDrawable*) metalDrawable;
  return pMetalCppDrawable;
}
