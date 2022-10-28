#pragma once

#include <map>

#include "Engine/Render/Model/Material.hpp"

namespace SHV {
namespace Metal {
class RenderPipeline;
class LogicalDevice;

class RenderPipelineCache {
   public:
    RenderPipelineCache(LogicalDevice& logicalDevice);
    ~RenderPipelineCache();

    const std::shared_ptr<RenderPipeline> Get(eMaterialShader shader);

   private:
    std::shared_ptr<RenderPipeline> CreateRenderPipeline(
        eMaterialShader shader);

    std::map<eMaterialShader, std::shared_ptr<RenderPipeline>> cache;

    LogicalDevice& logicalDevice;
};
}  // namespace Metal
}  // namespace SHV