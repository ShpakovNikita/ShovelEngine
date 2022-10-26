#include "Engine/Render/Metal/Model/RenderMaterial.hpp"

#include "Engine/Render/Metal/Model/GPUTexture.hpp"
#include "Engine/Render/Metal/RenderPipeline.hpp"

#include "Engine/Render/Metal/Shaders/ShaderDefinitions.h"
#include "Engine/Render/Metal/Shaders/PbrDefinitions.h"

#include "Engine/Common/Assert.hpp"

using namespace SHV;

namespace SHV::Metal::SRenderMaterial {
static const std::map<std::string, int> paramsToLocation = {
    {"baseColor", BaseColorTexture}};
}

Metal::RenderMaterial::RenderMaterial(
    std::shared_ptr<RenderPipeline> aRenderPipeline)
    : pipeline(aRenderPipeline) {}

Metal::RenderMaterial::~RenderMaterial() = default;

void Metal::RenderMaterial::Bind(MTL::RenderCommandEncoder& encoder) {
    for (auto& [paramName, texture] : textures) {
        auto textureLocation =
            SRenderMaterial::paramsToLocation.find(paramName);
        AssertD(textureLocation != SRenderMaterial::paramsToLocation.end());

        // TODO: pass location
        texture->Bind(encoder, textureLocation->second);
    }
}

Metal::RenderPipeline& Metal::RenderMaterial::GetRenderPipeline() const {
    return *pipeline;
}

void Metal::RenderMaterial::SetTexture(const std::string& paramName,
                                       std::shared_ptr<GPUTexture> texture) {
    textures[paramName] = texture;
}
