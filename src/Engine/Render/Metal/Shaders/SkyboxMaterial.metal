#include <metal_stdlib>

#include "ShaderDefinitions.h"
#include "SkyboxDefinitions.h"

using namespace SHV::Metal;
using namespace metal;

struct VertexOut {
    float4 pos [[position]];
    float4 texCoords;
};

vertex VertexOut SkyboxVertex(const device SkyboxVertexLayout *vertexArray [[buffer(0)]], const device UniformsData &uniformsData [[buffer(1)]], unsigned int vid [[vertex_id]]) {
    SkyboxVertexLayout in = vertexArray[vid];

    VertexOut out;

    float4 outPos = uniformsData.projection * uniformsData.viewRotation * in.pos;
    out.pos = outPos.xyww;
    out.texCoords = in.pos;
    return out;
}

fragment float4 SkyboxFragment(VertexOut interpolated [[stage_in]], texturecube<float, access::sample> colorTexture [[ texture(SkyboxColorTexture) ]], sampler textureSampler [[ sampler(SkyboxColorTexture) ]]) {
    float3 direction = normalize(interpolated.texCoords.rgb);
    const float4 colorSample = colorTexture.sample(textureSampler, direction);
    return colorSample;
}

float2 SampleSphericalMap(float3 direction) {
    const float2 invAtan = float2(0.1591, 0.3183);
    float2 uv = float2(atan2(direction.z, direction.x), asin(direction.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

fragment float4 SkyboxEquirectangularFragment(VertexOut interpolated [[stage_in]], texture2d<float, access::sample> colorTexture [[ texture(SkyboxColorTexture) ]], sampler textureSampler [[ sampler(SkyboxColorTexture) ]]) {
    float3 direction = normalize(interpolated.texCoords.rgb);
    float2 uv = SampleSphericalMap(direction);
    const float4 colorSample = colorTexture.sample(textureSampler, uv);
    return colorSample;
}
