#pragma once

#include <string>

namespace MTL {
class RenderPipelineState;
}

namespace SHV {
namespace Metal {

struct RenderPipelineParams {
    bool enableDepthTest = true;
};

class LogicalDevice;

class RenderPipeline {
   public:
    RenderPipeline(LogicalDevice& logicalDevice,
                   const std::string& vertexProgramName,
                   const std::string& fragmentProgramName);

    RenderPipeline(LogicalDevice& logicalDevice,
                   const std::string& vertexProgramName,
                   const std::string& fragmentProgramName,
                   const RenderPipelineParams& renderPipelineParams);
    ~RenderPipeline();

    void SetUp();
    void TearDown();

    MTL::RenderPipelineState& GetRenderPipelineState() const;

   private:
    LogicalDevice& logicalDevice;
    MTL::RenderPipelineState* pipelineState = nullptr;

    std::string vertexProgramName;
    std::string fragmentProgramName;

    const RenderPipelineParams renderPipelineParams;
};
}  // namespace Metal
}  // namespace SHV
