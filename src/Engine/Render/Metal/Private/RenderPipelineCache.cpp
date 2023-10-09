#include "Engine/Render/Metal/RenderPipelineCache.hpp"

#include "Engine/Render/Metal/RenderPipeline.hpp"
#include "Engine/Render/Metal/LogicalDevice.hpp"

using namespace SHV;

Metal::RenderPipelineCache::RenderPipelineCache(LogicalDevice& aLogicalDevice)
    : logicalDevice(aLogicalDevice){};

Metal::RenderPipelineCache::~RenderPipelineCache() = default;

const std::shared_ptr<Metal::RenderPipeline> Metal::RenderPipelineCache::Get(
    eMaterialShader shader) {
    auto it = cache.find(shader);
    if (it != cache.end()) {
        return it->second;
    }

    auto shaderProgram = CreateRenderPipeline(shader);
    cache[shader] = shaderProgram;
    return shaderProgram;
}

std::shared_ptr<Metal::RenderPipeline>
Metal::RenderPipelineCache::CreateRenderPipeline(eMaterialShader shader) {
    switch (shader) {
        case eMaterialShader::kPbrShader:
            return std::make_shared<RenderPipeline>(logicalDevice, "pbr_vertex",
                                                    "pbr_fragment");
        case eMaterialShader::kBasicShader:
            return std::make_shared<RenderPipeline>(
                logicalDevice, "basic_vertex", "basic_fragment");
        case eMaterialShader::kSkyboxShader:
            return std::make_shared<RenderPipeline>(
                logicalDevice, "skybox_vertex", "skybox_fragment");
        case eMaterialShader::kSkyboxEquirectangularShader:
            return std::make_shared<RenderPipeline>(
                logicalDevice, "skybox_vertex", "skybox_equirectangular_fragment");
    }
}
