#version 460 core

layout (location = 0) in vec2 v_pos;
layout (location = 1) in vec2 v_texCoords;

uniform mat3 v_model;
uniform mat3 v_proj;
uniform mat3 v_view;

out vec2 f_texCoords;

void main()
{
	gl_Position = mat4(v_proj) * mat4(v_view) * mat4(v_model) * vec4(v_pos, 1.0, 1.0);
	f_texCoords = v_texCoords;
}
