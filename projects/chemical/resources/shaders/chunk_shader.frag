#version 460 core

out vec4 fragColor;

in vec3 f_colour;

void main()
{                          

    fragColor = vec4(f_colour,1.0);
}