#version 330 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec2 aNormal;
layout (location = 2) in vec2 aUv;

struct Vertex {
    vec4 color;
    vec2 uv;
};

out Vertex vertex;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vec4 pos = vec4(aPos.x, aPos.y, aPos.z, 1.0);

    vertex.color = pos;
    vertex.uv = aUv;

    gl_Position = projection * view * model * pos;
}