#version 460 core

out vec4 fragColor;

in vec3 f_normal;

void main()
{                          

    fragColor = vec4(f_normal,1.0);
}