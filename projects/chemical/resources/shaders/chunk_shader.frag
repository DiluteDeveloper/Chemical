#version 460 core

out vec4 fragColor;

in vec3 f_pos;

void main()
{                          

    vec3 fixed_pos = (f_pos / 720) + 0.1;
    fixed_pos.y = 0;
    fragColor = vec4(fixed_pos,1.0);
}