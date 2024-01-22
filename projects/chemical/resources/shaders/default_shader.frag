#version 460 core

out vec4 fragColor;

in vec3 f_col;

void main()
{                          

    fragColor = vec4(f_col,1.0);
}