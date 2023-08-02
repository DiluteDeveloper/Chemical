#version 460 core

layout (location = 0) in vec3 v_pos;

uniform mat4 v_model;
uniform mat4 v_proj;
uniform mat4 v_view;

out vec3 f_pos;

void main()
{
	gl_Position = v_proj * v_view * v_model * vec4(v_pos, 1.0);
	f_pos = v_pos;
}