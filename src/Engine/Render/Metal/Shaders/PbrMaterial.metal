#import <metal_stdlib>

#include "ShaderDefinitions.h"
#include "PbrDefinitions.h"

using namespace SHV::Metal;
using namespace metal;

struct VertexOut {
    float4 color;
    float4 pos [[position]];
    float2 uv;
};

vertex VertexOut pbr_vertex(const device BasicVertexLayout *vertexArray [[buffer(0)]], const device UniformsData &uniformsData [[buffer(1)]], unsigned int vid [[vertex_id]]) {
  BasicVertexLayout in = vertexArray[vid];

  VertexOut out;

  out.color = in.pos;
  out.uv = in.uv;
  out.pos = uniformsData.projection * uniformsData.view * uniformsData.model * in.pos;

  return out;
}

fragment float4 pbr_fragment(VertexOut interpolated [[stage_in]], texture2d<half, access::sample> colorTexture [[ texture(BaseColorTexture) ]], sampler textureSampler [[ sampler(BaseColorTexture) ]]) {
    const half4 colorSample = colorTexture.sample(textureSampler, interpolated.uv);

    return float4(colorSample.rgba);
}

