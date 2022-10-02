#version 330 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec2 aNormal;
layout (location = 2) in vec2 aUv;

struct Vertex {
    vec4 color;
};

out Vertex vertex;

void main()
{
    vec4 pos = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    
    vertex.color = pos;

    gl_Position = pos;
}