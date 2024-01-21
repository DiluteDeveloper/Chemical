#version 460 core

layout(location = 0) in uint v_bit_data;

uniform ivec2 v_chunk_origin;
uniform mat4 v_proj;
uniform mat4 v_view;

out flat unsigned int texID;
out flat unsigned int faceType;
out vec2 texCoord;

vec3 GetBlockPositionWithinChunk() {

	vec3 pos;

	pos.x = (v_bit_data >> 17) & 0x0000001F;
	pos.y = (v_bit_data >> 8) & 0x0000001FF;
	pos.z = (v_bit_data >> 3) & 0x0000001F;

	return pos;
}

unsigned int GetBlockTextureID() {

	switch(v_bit_data >> 22) {
	case 1: // stone
		return 0;
		break;
	case 2: // dirt
		return 1;
		break;
	case 3: // grass
		return 2;
		break;
	case 4: // bedrock
		return 3;
		break;

	}

	// very stupid code but i just want to see it work
	if(v_bit_data >> 22 == 5) {
		switch(v_bit_data & 0x00000007) {
			case 0:
				return 4;
				break;
			case 1:
				return 4;
				break;
			case 2:
				return 5;
				break;
			case 3:
				return 5;
				break;
			case 4:
				return 5;
				break;
			case 5:
				return 5;
				break;
		}
	}

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
			return vec3(1.0, 0.0, 1.0);
		if (gl_VertexID % 6 == 4)
			return vec3(1.0, 1.0, 1.0);
		if (gl_VertexID % 6 == 5)
			return vec3(1.0, 1.0, 0.0);
		break;
	case 3: // left
		if (gl_VertexID % 6 == 0)
			return vec3(0.0, 1.0, 0.0);
		if (gl_VertexID % 6 == 1)
			return vec3(0.0, 0.0, 1.0);
		if (gl_VertexID % 6 == 2)
			return vec3(0.0, 0.0, 0.0);
		if (gl_VertexID % 6 == 3)
			return vec3(0.0, 1.0, 0.0);
		if (gl_VertexID % 6 == 4)
			return vec3(0.0, 1.0, 1.0);
		if (gl_VertexID % 6 == 5)
			return vec3(0.0, 0.0, 1.0);
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

vec2 GetVertexTexCoord() {

	switch(v_bit_data & 0x00000007) {
	case 0: // top
		if (gl_VertexID % 6 == 0)
			return vec2(0.0, 1.0);
		if (gl_VertexID % 6 == 1)
			return vec2(1.0, 1.0);
		if (gl_VertexID % 6 == 2)
			return vec2(0.0, 0.0);
		if (gl_VertexID % 6 == 3)
			return vec2(0.0, 0.0);
		if (gl_VertexID % 6 == 4)
			return vec2(1.0, 1.0);
		if (gl_VertexID % 6 == 5)
			return vec2(1.0, 0.0);
		break;
	
	case 1: // bottom
		if (gl_VertexID % 6 == 0)
			return vec2(0.0, 1.0);
		if (gl_VertexID % 6 == 1)
			return vec2(1.0, 0.0);
		if (gl_VertexID % 6 == 2)
			return vec2(0.0, 0.0);
		if (gl_VertexID % 6 == 3)
			return vec2(1.0, 1.0);
		if (gl_VertexID % 6 == 4)
			return vec2(1.0, 0.0);
		if (gl_VertexID % 6 == 5)
			return vec2(0.0, 1.0);
		break;
	case 2: // right
		if (gl_VertexID % 6 == 0)
			return vec2(0.0, 1.0);
		if (gl_VertexID % 6 == 1)
			return vec2(1.0, 1.0);
		if (gl_VertexID % 6 == 2)
			return vec2(0.0, 0.0);
		if (gl_VertexID % 6 == 3)
			return vec2(1.0, 1.0);
		if (gl_VertexID % 6 == 4)
			return vec2(1.0, 0.0);
		if (gl_VertexID % 6 == 5)
			return vec2(0.0, 0.0);
		break;
	case 3: // left
		if (gl_VertexID % 6 == 0)
			return vec2(0.0, 0.0);
		if (gl_VertexID % 6 == 1)
			return vec2(1.0, 1.0);
		if (gl_VertexID % 6 == 2)
			return vec2(0.0, 1.0);
		if (gl_VertexID % 6 == 3)
			return vec2(0.0, 0.0);
		if (gl_VertexID % 6 == 4)
			return vec2(1.0, 0.0);
		if (gl_VertexID % 6 == 5)
			return vec2(1.0, 1.0);
		break;
	case 4: // front
		if (gl_VertexID % 6 == 0)
			return vec2(1.0, 1.0);
		if (gl_VertexID % 6 == 1)
			return vec2(0.0, 1.0);
		if (gl_VertexID % 6 == 2)
			return vec2(0.0, 0.0);
		if (gl_VertexID % 6 == 3)
			return vec2(1.0, 1.0);
		if (gl_VertexID % 6 == 4)
			return vec2(0.0, 0.0);
		if (gl_VertexID % 6 == 5)
			return vec2(1.0, 0.0);
		break;
	case 5: // back
		if (gl_VertexID % 6 == 0)
			return vec2(1.0, 1.0);
		if (gl_VertexID % 6 == 1)
			return vec2(0.0, 1.0);
		if (gl_VertexID % 6 == 2)
			return vec2(1.0, 0.0);
		if (gl_VertexID % 6 == 3)
			return vec2(0.0, 0.0);
		if (gl_VertexID % 6 == 4)
			return vec2(1.0, 0.0);
		if (gl_VertexID % 6 == 5)
			return vec2(0.0, 1.0);
		break;
	};
}



void main()
{

	faceType = v_bit_data & 0x00000007;
	texID = GetBlockTextureID();
	texCoord = GetVertexTexCoord();
	vec3 v_posi = GetVertexPosition();
	vec3 rel_posi = GetBlockPositionWithinChunk();

	gl_Position = v_proj * v_view * vec4(v_chunk_origin.x + rel_posi.x + v_posi.x, v_posi.y + rel_posi.y, v_chunk_origin.y + rel_posi.z + v_posi.z, 1.0);
}