#version 330 core

layout (location = 0) in vec4 aPos;
layout (location = 2) in vec2 aUv;

struct Vertex {
    vec2 uv;
};

out Vertex vertex;

uniform mat4 view;

void main()
{
    vec4 pos = vec4(aPos.x, aPos.y, aPos.z, 1.0);

    vertex.uv = aUv;

    gl_Position = view * pos;
}