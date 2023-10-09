#version 330 core

#define M_PI 3.14159265358979323846

out vec4 FragColor;

struct Vertex {
    vec3 texCoords;
};

in Vertex vertex;

uniform sampler2D skyboxMap;

void main()
{
    vec3 direction = normalize(vertex.texCoords.rgb);

    float u = atan(direction.z, direction.x) / (2.0 * M_PI) + 0.5;
    float v = asin(direction.y) / M_PI + 0.5;

    vec4 skyboxColor = texture(skyboxMap, vec2(u, v));
    FragColor = skyboxColor;
}