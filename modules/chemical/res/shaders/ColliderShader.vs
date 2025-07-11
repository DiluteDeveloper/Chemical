#version 460 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_normal;

uniform vec3 v_collider_position;
uniform mat4 v_view;
uniform mat4 v_proj;

void main()
{
    gl_Position = v_proj * v_view * vec4(v_collider_position + v_pos, 1.0);
}
