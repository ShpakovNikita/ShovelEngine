#pragma once

#include <memory>

namespace MTL {
class Texture;
class RenderCommandEncoder;
class BlitCommandEncoder;
class SamplerState;
class TextureDescriptor;
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
    void CreateMetalTexture(const std::shared_ptr<Texture>& texture,
                            LogicalDevice& logicalDevice,
                            MTL::BlitCommandEncoder& blitCommandEncoder);
    MTL::TextureDescriptor* CreateTextureDescriptor(const std::shared_ptr<Texture>& texture);
    MTL::TextureDescriptor* CreateBaseTextureDescriptor(const std::shared_ptr<Texture>& texture);

    void CopyDataTextureData(const std::shared_ptr<Texture>& source, MTL::Texture* target);
    void CopyDataTexture2DData(const std::shared_ptr<Texture>& source, MTL::Texture* target);
    void CopyDataTextureCubeData(const std::shared_ptr<Texture>& source, MTL::Texture* target);

    std::weak_ptr<Texture> texture;
    MTL::Texture* metalTexture;
    MTL::SamplerState* metalSamplerState;
    LogicalDevice& logicalDevice;
};
}  // namespace Metal
}  // namespace SHV