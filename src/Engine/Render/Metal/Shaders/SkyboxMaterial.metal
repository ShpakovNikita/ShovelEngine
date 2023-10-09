#include <metal_stdlib>

#include "ShaderDefinitions.h"
#include "SkyboxDefinitions.h"

#define M_PI 3.14159265358979323846

using namespace SHV::Metal;
using namespace metal;

struct VertexOut {
    float4 pos [[position]];
    float4 texCoords;
};

vertex VertexOut skybox_vertex(const device SkyboxVertexLayout *vertexArray [[buffer(0)]], const device UniformsData &uniformsData [[buffer(1)]], unsigned int vid [[vertex_id]]) {
    SkyboxVertexLayout in = vertexArray[vid];

    VertexOut out;

    float4 outPos = uniformsData.projection * uniformsData.viewRotation * in.pos;
    out.pos = outPos.xyww;
    out.texCoords = in.pos;
    return out;
}

fragment float4 skybox_fragment(VertexOut interpolated [[stage_in]], texturecube<float, access::sample> colorTexture [[ texture(SkyboxColorTexture) ]], sampler textureSampler [[ sampler(SkyboxColorTexture) ]]) {
    float3 direction = normalize(interpolated.texCoords.rgb);
    const float4 colorSample = colorTexture.sample(textureSampler, direction);
    return colorSample;
}

fragment float4 skybox_equirectangular_fragment(VertexOut interpolated [[stage_in]], texture2d<float, access::sample> colorTexture [[ texture(SkyboxColorTexture) ]], sampler textureSampler [[ sampler(SkyboxColorTexture) ]]) {
    float3 direction = normalize(interpolated.texCoords.rgb);

    float u = atan2(direction.z, direction.x) / (2.0 * M_PI) + 0.5;
    float v = asin(direction.y) / M_PI + 0.5;

    const float4 colorSample = colorTexture.sample(textureSampler, float2(u, v));
    return colorSample;
}
