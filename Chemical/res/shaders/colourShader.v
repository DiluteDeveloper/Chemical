#version 460 core

layout (location = 0) in vec3 v_pos;

uniform mat4 v_model;
uniform mat4 v_view;
uniform mat4 v_proj;

void main()
{
   gl_Position = v_proj * v_view * v_model * vec4(v_pos.xyz, 1.0);
}