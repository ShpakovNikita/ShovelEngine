#pragma once

#include <string>

namespace MTL {
class RenderPipelineState;
}

namespace SHV {
namespace Metal {

class LogicalDevice;

class RenderPipeline {
   public:
    RenderPipeline(LogicalDevice& logicalDevice,
                   const std::string& vertexProgramName,
                   const std::string& fragmentProgramName);

    void SetUp();
    void TearDown();

    MTL::RenderPipelineState& GetRenderPipelineState() const;

   private:
    LogicalDevice& logicalDevice;
    MTL::RenderPipelineState* pipelineState = nullptr;

    std::string vertexProgramName;
    std::string fragmentProgramName;
};
}  // namespace Metal
}  // namespace SHV
