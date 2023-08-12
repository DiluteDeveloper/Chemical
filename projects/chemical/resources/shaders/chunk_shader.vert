#version 460 core

layout(location = 0) in uint v_bit_data;


uniform ivec2 v_chunk_origin;
uniform mat4 v_proj;
uniform mat4 v_view;

out vec3 f_colour;

vec3 GetBlockPositionWithinChunk() {

	vec3 pos;

	pos.x = (v_bit_data >> 15) & 0x0000001F;
	pos.y = (v_bit_data >> 8) & 0x00000007F;
	pos.z = (v_bit_data >> 3) & 0x0000001F;

	return pos;
}

vec3 GetBlockColour() {

	switch(v_bit_data >> 20) {
	case 1: // stone
		return vec3(0.38, 0.38, 0.38);
		break;
	case 2: // dirt
		return vec3(0.6, 0.3, 0.1);
		break;
	case 3: // grass
		return vec3(0, 1, 0);
		break;
	case 4: // bedrock
		return vec3(0.2, 0.2, 0.2);
		break;
	};
}

vec3 GetVertexPosition() {


	switch(v_bit_data & 0x00000007) {
	case 0: // top
		if (gl_VertexID % 6 == 0)
			return vec3(0.0, 1.0, 0.0);
		if (gl_VertexID % 6 == 1)
			return vec3(1.0, 1.0, 0.0);
		if (gl_VertexID % 6 == 2)
			return vec3(0.0, 1.0, 1.0);
		if (gl_VertexID % 6 == 3)
			return vec3(0.0, 1.0, 1.0);
		if (gl_VertexID % 6 == 4)
			return vec3(1.0, 1.0, 0.0);
		if (gl_VertexID % 6 == 5)
			return vec3(1.0, 1.0, 1.0);
		break;
	case 1: // bottom
		if (gl_VertexID % 6 == 0)
			return vec3(0.0, 0.0, 1.0);
		if (gl_VertexID % 6 == 1)
			return vec3(1.0, 0.0, 0.0);
		if (gl_VertexID % 6 == 2)
			return vec3(0.0, 0.0, 0.0);
		if (gl_VertexID % 6 == 3)
			return vec3(1.0, 0.0, 1.0);
		if (gl_VertexID % 6 == 4)
			return vec3(1.0, 0.0, 0.0);
		if (gl_VertexID % 6 == 5)
			return vec3(0.0, 0.0, 1.0);
		break;
	case 2: // right
		if (gl_VertexID % 6 == 0)
			return vec3(1.0, 0.0, 0.0);
		if (gl_VertexID % 6 == 1)
			return vec3(1.0, 0.0, 1.0);
		if (gl_VertexID % 6 == 2)
			return vec3(1.0, 1.0, 0.0);
		if (gl_VertexID % 6 == 3)
			return vec3(1.0, 1.0, 1.0);
		if (gl_VertexID % 6 == 4)
			return vec3(1.0, 1.0, 0.0);
		if (gl_VertexID % 6 == 5)
			return vec3(1.0, 0.0, 1.0);
		break;
	case 3: // left
		if (gl_VertexID % 6 == 0)
			return vec3(0.0, 1.0, 0.0);
		if (gl_VertexID % 6 == 1)
			return vec3(0.0, 0.0, 1.0);
		if (gl_VertexID % 6 == 2)
			return vec3(0.0, 0.0, 0.0);
		if (gl_VertexID % 6 == 3)
			return vec3(0.0, 0.0, 1.0);
		if (gl_VertexID % 6 == 4)
			return vec3(0.0, 1.0, 0.0);
		if (gl_VertexID % 6 == 5)
			return vec3(0.0, 1.0, 1.0);
		break;
	case 4: // front
		if (gl_VertexID % 6 == 0)
			return vec3(1.0, 0.0, 1.0);
		if (gl_VertexID % 6 == 1)
			return vec3(0.0, 0.0, 1.0);
		if (gl_VertexID % 6 == 2)
			return vec3(0.0, 1.0, 1.0);
		if (gl_VertexID % 6 == 3)
			return vec3(1.0, 0.0, 1.0);
		if (gl_VertexID % 6 == 4)
			return vec3(0.0, 1.0, 1.0);
		if (gl_VertexID % 6 == 5)
			return vec3(1.0, 1.0, 1.0);
		break;
	case 5: // back
		if (gl_VertexID % 6 == 0)
			return vec3(0.0, 0.0, 0.0);
		if (gl_VertexID % 6 == 1)
			return vec3(1.0, 0.0, 0.0);
		if (gl_VertexID % 6 == 2)
			return vec3(0.0, 1.0, 0.0);
		if (gl_VertexID % 6 == 3)
			return vec3(1.0, 1.0, 0.0);
		if (gl_VertexID % 6 == 4)
			return vec3(0.0, 1.0, 0.0);
		if (gl_VertexID % 6 == 5)
			return vec3(1.0, 0.0, 0.0);
		break;
	};
}





void main()
{


	f_colour = GetBlockColour();
	vec3 v_posi = GetVertexPosition();
	vec3 rel_posi = GetBlockPositionWithinChunk();

	gl_Position = v_proj * v_view * vec4(v_chunk_origin.x + rel_posi.x + v_posi.x, v_posi.y + rel_posi.y, v_chunk_origin.y + rel_posi.z + v_posi.z, 1.0);
}