#version 460 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec2 v_texCoord;
//layout (location = 2) in uint v_materialIndex;

//uniform mat4 v_model;

out vec2 f_texCoord;
//flat out uint f_materialIndex;

void main()
{
	gl_Position = vec4(v_pos, 1.0);
	f_texCoord = v_texCoord;
	//f_materialIndex = v_materialIndex;
}