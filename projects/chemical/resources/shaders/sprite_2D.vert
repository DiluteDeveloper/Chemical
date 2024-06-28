#version 460 core

layout (location = 0) in vec2 vertex_position;

uniform vec2 real_position;
uniform uint zIndex;
uniform vec2 scale;

void main()
{
	gl_Position = vec4((real_position.x + vertex_position.x) * scale.x,
					   (real_position.y + vertex_position.y) * scale.y,
					   zIndex, 1.0);
}