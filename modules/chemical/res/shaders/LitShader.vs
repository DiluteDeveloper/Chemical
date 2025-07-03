#version 460 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_normal;

uniform mat4 v_model;
uniform mat4 v_view;
uniform mat4 v_proj;

out vec3 f_normal;
out vec3 f_fragPos;

void main()
{
    
    gl_Position = v_proj * v_view * v_model * vec4(v_pos, 1.0);
    //f_normal = normalize(mat3(transpose(inverse(v_model))) * v_normal);
    f_normal = v_normal;
    f_fragPos = vec3(v_pos);
}
