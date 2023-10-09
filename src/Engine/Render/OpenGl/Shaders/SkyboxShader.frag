#version 330 core
out vec4 FragColor;

struct Vertex {
    vec2 uv;
};

in Vertex vertex;

uniform sampler2D skyboxMap;

void main()
{
    vec4 baseColor = texture(skyboxMap, vertex.uv);
    FragColor = baseColor;
}