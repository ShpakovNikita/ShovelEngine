#include "ShaderDefinitions.h"

using namespace SHV::Metal;

struct VertexOut {
    float4 color;
    float4 pos [[position]];
};

vertex VertexOut basic_vertex(const device BasicVertexLayout *vertexArray [[buffer(0)]], const device UniformsData &uniformsData [[buffer(1)]], unsigned int vid [[vertex_id]]) {
  BasicVertexLayout in = vertexArray[vid];

  VertexOut out;

  out.color = in.pos;
  out.pos = uniformsData.projection * uniformsData.view * uniformsData.model * in.pos;

  return out;
}

fragment float4 basic_fragment(VertexOut interpolated [[stage_in]]) {
    return interpolated.color;
}

