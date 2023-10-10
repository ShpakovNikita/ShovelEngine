#version 330 core

#define M_PI 3.14159265358979323846

out vec4 FragColor;

struct Vertex {
    vec3 texCoords;
};

in Vertex vertex;

uniform sampler2D skyboxMap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{
    vec3 direction = normalize(vertex.texCoords.rgb);
    vec2 uv = SampleSphericalMap(direction);
    vec4 skyboxColor = texture(skyboxMap, uv);
    FragColor = skyboxColor;
}