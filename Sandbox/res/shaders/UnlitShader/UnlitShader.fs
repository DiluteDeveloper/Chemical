#version 460 core

out vec4 fragColor;

struct Material {
    vec3 diffuse;
};
uniform Material material;

void main()
{
    fragColor = vec4(material.diffuse, 1.0);
}