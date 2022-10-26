#version 330 core
out vec4 FragColor;

struct Vertex {
    vec4 color;
    vec2 uv;
};

in Vertex vertex;

uniform sampler2D albedoMap;

void main()
{
    vec4 baseColor = texture(albedoMap, vertex.uv);
    FragColor = baseColor;
} 