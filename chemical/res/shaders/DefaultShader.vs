#version 460 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec2 v_texCoord;
layout (location = 2) in vec3 v_normal;

uniform mat4 v_model;
uniform mat4 v_proj;
uniform mat4 v_view;

out vec3 f_normal;
out vec3 f_pos;

out vec2 f_texCoord;
void main()
{
	gl_Position = v_proj * v_view * v_model * vec4(v_pos, 1.0);
	f_texCoord = v_texCoord;

	f_normal = transpose(inverse(mat3(v_model))) * v_normal;
	f_pos = vec3(v_model * vec4(v_pos, 1.0));
}