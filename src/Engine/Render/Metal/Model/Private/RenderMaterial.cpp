#include "Engine/Render/Metal/Model/RenderMaterial.hpp"

#include "Engine/Render/Metal/Model/GPUTexture.hpp"
#include "Engine/Render/Metal/RenderPipeline.hpp"

#include "Engine/Render/Metal/Shaders/ShaderDefinitions.h"
#include "Engine/Render/Metal/Shaders/PbrDefinitions.h"

#include "Engine/Common/Assert.hpp"
#include "Engine/Common/Logger.hpp"

using namespace SHV;

namespace SHV::Metal::SRenderMaterial {
static const std::map<std::string, int> paramsToLocation = {
    {"albedoMap", BaseColorTexture},
    {"aoMap", AmbientOcclusionTexture},
    {"heightMap", HeightMapTexture},
    {"metallicMap", MetallicRoughnessTexture},
    {"normalMap", NormalTexture}};
}

Metal::RenderMaterial::RenderMaterial(
    std::shared_ptr<RenderPipeline> aRenderPipeline)
    : pipeline(aRenderPipeline) {}

Metal::RenderMaterial::~RenderMaterial() = default;

void Metal::RenderMaterial::Bind(MTL::RenderCommandEncoder& encoder) {
    if (invalidate) {
        Invalidate();
    }

    for (auto& [paramName, texture] : textures) {
        auto textureLocation =
            SRenderMaterial::paramsToLocation.find(paramName);

        // TODO: not set unused textures in pipeline
        if (textureLocation != SRenderMaterial::paramsToLocation.end()) {
            texture->Bind(encoder, textureLocation->second);
        }
    }
}

Metal::RenderPipeline& Metal::RenderMaterial::GetRenderPipeline() const {
    return *pipeline;
}

void Metal::RenderMaterial::SetTexture(const std::string& paramName,
                                       std::shared_ptr<GPUTexture> texture) {
    textures[paramName] = texture;
    invalidate = true;
}

void Metal::RenderMaterial::Invalidate() {
    AssertD(invalidate == true);

    for (auto& [paramName, texture] : textures) {
        auto textureLocation =
            SRenderMaterial::paramsToLocation.find(paramName);

        if (textureLocation == SRenderMaterial::paramsToLocation.end()) {
            LogW(eTag::kMetalAPI)
                << "Failed to find texture location for param name "
                << paramName << std::endl;
        }
    }

    invalidate = false;
}
