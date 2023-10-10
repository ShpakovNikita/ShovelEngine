#version 330 core
out vec4 FragColor;

struct Vertex {
    vec3 texCoords;
};

in Vertex vertex;

uniform samplerCube skyboxMap;

void main()
{
    vec3 direction = normalize(vertex.texCoords.xyz);
    vec4 baseColor = texture(skyboxMap, direction);
    FragColor = baseColor;
}