#version 460 core

layout (location = 0) in vec3 v_pos;
layout(location = 1) in vec3 v_colour;

uniform dvec3 v_chunk_origin;
uniform mat4 v_proj;
uniform mat4 v_view;

out vec3 f_colour;

void main()
{
	gl_Position = v_proj * v_view * vec4(v_chunk_origin + v_pos, 1.0);
	f_colour = v_colour;  
}