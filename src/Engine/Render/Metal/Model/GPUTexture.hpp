#pragma once

#include <memory>

namespace MTL {
class Texture;
class RenderCommandEncoder;
class BlitCommandEncoder;
class SamplerState;
}  // namespace MTL

namespace SHV {
class Texture;

namespace Metal {
class LogicalDevice;

class GPUTexture {
   public:
    GPUTexture(std::weak_ptr<Texture> texture, LogicalDevice& logicalDevice,
               MTL::BlitCommandEncoder& blitCommandEncoder);
    ~GPUTexture();

    void Bind(MTL::RenderCommandEncoder& encoder, int location);
    std::shared_ptr<Texture> MakeTexture() const;

   private:
    std::weak_ptr<Texture> texture;
    MTL::Texture* metalTexture;
    MTL::SamplerState* metalSamplerState;
    LogicalDevice& logicalDevice;
};
}  // namespace Metal
}  // namespace SHV