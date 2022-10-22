#version 330 core
out vec4 FragColor;

struct Vertex {
    vec4 color;
    vec2 uv;
};

in Vertex vertex;

uniform sampler2D baseColor;

void main()
{
    vec4 baseColor = texture(baseColor, vertex.uv);
    FragColor = baseColor;
} 