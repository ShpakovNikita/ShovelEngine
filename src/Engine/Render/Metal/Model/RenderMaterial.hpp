#pragma once

#include <map>
#include <memory>

namespace MTL {
class RenderCommandEncoder;
}

namespace SHV {
namespace Metal {
class RenderPipeline;
class GPUTexture;

class RenderMaterial {
   public:
    RenderMaterial(std::shared_ptr<RenderPipeline> pipeline);
    ~RenderMaterial();

    void Bind(MTL::RenderCommandEncoder& encoder);

    void SetTexture(const std::string& paramName,
                    std::shared_ptr<GPUTexture> texture);

    RenderPipeline& GetRenderPipeline() const;

   private:
    std::map<std::string, std::shared_ptr<GPUTexture>> textures;
    std::shared_ptr<RenderPipeline> pipeline;
};
}  // namespace Metal
}  // namespace SHV