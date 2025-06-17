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


void main()
{
	gl_Position =  vec4(GetVertexPosition(), 1.0, 1.0);
}
