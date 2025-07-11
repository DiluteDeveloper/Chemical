#version 460 core

uniform vec3 f_colour;

out vec4 fragColor;

void main()
{                          
    fragColor = vec4(f_colour, 1.0);
}
