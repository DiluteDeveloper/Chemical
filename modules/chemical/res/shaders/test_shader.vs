#version 460 core

layout (location = 0) in vec2 v_pos;

uniform mat3 v_model;
// uniform mat4 v_view;


void main()
{
	gl_Position = mat4(v_model) * vec4(v_pos, 1.0, 1.0);
}
