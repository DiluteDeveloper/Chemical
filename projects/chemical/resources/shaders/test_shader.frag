#version 460 core

in vec3 f_normal;

out vec4 fragColor;

void main()
{                          

    fragColor = vec4(f_normal, 1.0);
}