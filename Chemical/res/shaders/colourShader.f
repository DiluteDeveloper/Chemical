#version 460 core

out vec4 fragColor;

uniform vec3 f_colour;

void main()
{
    fragColor = vec4(f_colour.xyz, 1.0);
}