#version 460 core

layout (location = 0) in vec2 v_pos;
layout (location = 1) in vec3 v_colour;
layout (location = 2) in vec2 v_texCoord;

out vec3 f_colour;
out vec2 f_texCoord;

void main()
{
	gl_Position = vec4(v_pos, 0.0, 1.0);
	f_colour = v_colour;
	f_texCoord = v_texCoord;
}