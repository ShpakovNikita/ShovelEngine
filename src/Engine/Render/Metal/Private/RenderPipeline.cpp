#include "Engine/Render/Metal/RenderPipeline.hpp"

#include "Engine/Common/Assert.hpp"
#include "Engine/Common/Logger.hpp"
#include "Engine/Common/Exception.hpp"
#include "Engine/Render/Metal/LogicalDevice.hpp"
#include "Metal/Metal.hpp"

using namespace SHV;

Metal::RenderPipeline::RenderPipeline(LogicalDevice& aLogicalDevice,
                                      const std::string& aVertexProgramName,
                                      const std::string& aFragmentProgramName)
    : Metal::RenderPipeline(aLogicalDevice, aVertexProgramName,
                            aFragmentProgramName, {}) {
    LogD(eTag::kMetalAPI) << "Creating up Render Pipeline for vertex: "
                          << aVertexProgramName
                          << " fragment: " << aFragmentProgramName << std::endl;

    MTL::Library* defaultLibrary =
        logicalDevice.GetDevice().newDefaultLibrary();
    AssertD(defaultLibrary != nullptr);

    NS::String* fragmentName =
        NS::String::string(fragmentProgramName.data(), NS::UTF8StringEncoding);
    MTL::Function* fragmentProgram = defaultLibrary->newFunction(fragmentName);

    NS::String* vertexName =
        NS::String::string(vertexProgramName.data(), NS::UTF8StringEncoding);
    MTL::Function* vertexProgram = defaultLibrary->newFunction(vertexName);

    MTL::RenderPipelineDescriptor* pipelineStateDescriptor =
        MTL::RenderPipelineDescriptor::alloc()->init();
    pipelineStateDescriptor->setVertexFunction(vertexProgram);
    pipelineStateDescriptor->setFragmentFunction(fragmentProgram);
    pipelineStateDescriptor->colorAttachments()->object(0)->setPixelFormat(
        MTL::PixelFormat::PixelFormatRGBA8Unorm);

    if (renderPipelineParams.enableDepthTest) {
        pipelineStateDescriptor->setDepthAttachmentPixelFormat(
            MTL::PixelFormat::PixelFormatDepth32Float_Stencil8);
    }

    NS::Error* error = nullptr;
    pipelineState = logicalDevice.GetDevice().newRenderPipelineState(
        pipelineStateDescriptor, &error);

    if (pipelineState == nullptr) {
        throw Exception("%s", error->localizedDescription()->utf8String());
    }

    fragmentProgram->release();
    vertexProgram->release();
    pipelineStateDescriptor->release();
    vertexName->release();
    fragmentName->release();
    defaultLibrary->release();
}

Metal::RenderPipeline::RenderPipeline(
    LogicalDevice& aLogicalDevice, const std::string& aVertexProgramName,
    const std::string& aFragmentProgramName,
    const RenderPipelineParams& aRenderPipelineParams)
    : logicalDevice(aLogicalDevice),
      vertexProgramName(aVertexProgramName),
      fragmentProgramName(aFragmentProgramName),
      renderPipelineParams(aRenderPipelineParams) {}

Metal::RenderPipeline::~RenderPipeline() {
    AssertD(pipelineState != nullptr);
    pipelineState->release();
    pipelineState = nullptr;

    LogD(eTag::kMetalAPI) << "Render Pipeline destroyed" << std::endl;
}

MTL::RenderPipelineState& Metal::RenderPipeline::GetRenderPipelineState()
    const {
    AssertD(pipelineState != nullptr);

    return *pipelineState;
}
