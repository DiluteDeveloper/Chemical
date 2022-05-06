#version 460 core

out vec4 fragColor;

uniform vec3 colour;

void main()
{
    fragColor = vec4(colour.xyz, 1.0);
}