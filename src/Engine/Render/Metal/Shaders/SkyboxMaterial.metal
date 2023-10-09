#include <metal_stdlib>

#include "ShaderDefinitions.h"
#include "SkyboxDefinitions.h"

#define M_PI 3.14159265358979323846

using namespace SHV::Metal;
using namespace metal;

struct VertexOut {
    float4 pos [[position]];
    float2 uv;
};

vertex VertexOut skybox_vertex(const device SkyboxVertexLayout *vertexArray [[buffer(0)]], const device UniformsData &uniformsData [[buffer(1)]], unsigned int vid [[vertex_id]]) {
    SkyboxVertexLayout in = vertexArray[vid];

    VertexOut out;

    out.pos = uniformsData.view * in.pos;
    out.uv = in.uv;

    return out;
}

fragment float4 skybox_fragment(VertexOut interpolated [[stage_in]], texturecube<float, access::sample> colorTexture [[ texture(SkyboxColorTexture) ]], sampler textureSampler [[ sampler(SkyboxColorTexture) ]]) {
    const float4 colorSample = colorTexture.sample(textureSampler, interpolated.pos.rgb);
    return float4(1.0, 1.0, 0.0, 0.0); //colorSample;
}

fragment float4 skybox_equirectangular_fragment(VertexOut interpolated [[stage_in]], texture2d<float, access::sample> colorTexture [[ texture(SkyboxColorTexture) ]], sampler textureSampler [[ sampler(SkyboxColorTexture) ]]) {
    float3 direction = normalize(interpolated.pos.rgb);

    float u = atan2(direction.z, direction.x) / (2.0 * M_PI) + 0.5;
    float v = asin(direction.y) / M_PI + 0.5;

    const float4 colorSample = colorTexture.sample(textureSampler, float2(u, v));
    return float4(1.0, 1.0, 0.0, 0.0); //colorSample;
}
