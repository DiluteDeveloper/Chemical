#version 460 core

layout (location = 0) in vec2 vertex_position;
layout (location = 1) in vec2 vertex_tex_coord;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform uint zIndex;

out vec2 f_vertex_tex_coord;

void main()
{
	f_vertex_tex_coord = vertex_tex_coord;
	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vertex_position, zIndex, 1.0);
}