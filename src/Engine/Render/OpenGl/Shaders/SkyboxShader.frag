#version 330 core
out vec4 FragColor;

struct Vertex {
    vec3 texCoords;
};

in Vertex vertex;

uniform sampler2D skyboxMap;

void main()
{
    vec4 baseColor = texture(skyboxMap, vertex.texCoords.xy);
    FragColor = baseColor;
}