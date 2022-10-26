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

    const std::shared_ptr<RenderPipeline> Get(eShader shader);

   private:
    std::shared_ptr<RenderPipeline> CreateRenderPipeline(eShader shader);

    std::map<eShader, std::shared_ptr<RenderPipeline>> cache;

    LogicalDevice& logicalDevice;
};
}  // namespace Metal
}  // namespace SHV