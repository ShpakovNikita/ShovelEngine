#version 330 core

layout (location = 0) in vec4 aPos;

struct Vertex {
    vec3 texCoords;
};

out Vertex vertex;

uniform mat4 viewRotation;
uniform mat4 projection;

void main()
{
    vec4 pos = projection * viewRotation * aPos;
    vertex.texCoords = aPos.xyz;
    gl_Position = pos.xyzw; // pos.xyww;
}