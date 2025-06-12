
#version 460 core

vec2 GetVertexPosition() {
	if(gl_VertexID == 0)
		return vec2(-0.5, -0.5);
	else if(gl_VertexID == 1)
		return vec2(-0.5, 0.5);
	else if(gl_VertexID == 2)
		return vec2(0.5, 0.5);
	else if(gl_VertexID == 3)
		return vec2(0.5, -0.5);
	else if(gl_VertexID == 4)
		return vec2(-0.5, -0.5);
	else if(gl_VertexID == 5)
		return vec2(0.5, 0.5);
}
vec2 GetVertexTexCoord() {
	if(gl_VertexID == 0)
		return vec2(0.0, 0.0);
	else if(gl_VertexID == 1)
		return vec2(0.0, 1.0);
	else if(gl_VertexID == 2)
		return vec2(1.0, 1.0);
	else if(gl_VertexID == 3)
		return vec2(1.0, 0.0);
	else if(gl_VertexID == 4)
		return vec2(0.0, 0.0);
	else if(gl_VertexID == 5)
		return vec2(1.0, 1.0);
}

uniform mat3 v_model;
uniform mat3 v_view;
uniform mat3 v_proj;

uniform uint v_frame_idx;

out vec2 f_texCoord;

void main()
{
	mat4 model = mat4(v_model);
	model[3][0] =cos(v_frame_idx / 40.0) * 5;
	model[3][1] =sin(v_frame_idx / 40.0) * 5;
	gl_Position = mat4(v_proj) * mat4(v_view) * model * vec4(GetVertexPosition(), 1.0, 1.0);
	f_texCoord = GetVertexTexCoord();
}
