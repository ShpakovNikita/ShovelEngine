#pragma once

#include <memory>

namespace MTL {
class Texture;
class RenderCommandEncoder;
class BlitCommandEncoder;
}  // namespace MTL

namespace SHV {
class Texture;

namespace Metal {
class LogicalDevice;

class GPUTexture {
   public:
    GPUTexture(const std::string& texturePath, LogicalDevice& logicalDevice,
               MTL::BlitCommandEncoder& blitCommandEncoder);
    ~GPUTexture();

    void Bind(MTL::RenderCommandEncoder& encoder, int location);

   private:
    std::weak_ptr<Texture> texture;
    MTL::Texture* metalTexture;
    LogicalDevice& logicalDevice;
};
}  // namespace Metal
}  // namespace SHV