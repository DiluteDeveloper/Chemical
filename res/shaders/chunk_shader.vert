#version 300 es

layout(location = 0) in uint v_bit_data;

uniform ivec2 v_chunk_origin;
uniform mat4 v_proj;
uniform mat4 v_view;

flat out uint texID;
flat out uint faceType;
out vec2 texCoord;

const vec3 POS[36] = vec3[36](
	// top
	vec3(0,1,0), vec3(1,1,0), vec3(0,1,1), vec3(0,1,1), vec3(1,1,0), vec3(1,1,1),
	// bottom
	vec3(0,0,1), vec3(1,0,0), vec3(0,0,0), vec3(1,0,1), vec3(1,0,0), vec3(0,0,1),
	// right
	vec3(1,0,0), vec3(1,0,1), vec3(1,1,0), vec3(1,0,1), vec3(1,1,1), vec3(1,1,0),
	// left
	vec3(0,1,0), vec3(0,0,1), vec3(0,0,0), vec3(0,1,0), vec3(0,1,1), vec3(0,0,1),
	// front
	vec3(1,0,1), vec3(0,0,1), vec3(0,1,1), vec3(1,0,1), vec3(0,1,1), vec3(1,1,1),
	// back
	vec3(0,0,0), vec3(1,0,0), vec3(0,1,0), vec3(1,1,0), vec3(0,1,0), vec3(1,0,0)
);

const vec2 UV[36] = vec2[36](
	// top
	vec2(0,1), vec2(1,1), vec2(0,0), vec2(0,0), vec2(1,1), vec2(1,0),
	// bottom
	vec2(0,1), vec2(1,0), vec2(0,0), vec2(1,1), vec2(1,0), vec2(0,1),
	// right
	vec2(0,1), vec2(1,1), vec2(0,0), vec2(1,1), vec2(1,0), vec2(0,0),
	// left
	vec2(0,0), vec2(1,1), vec2(0,1), vec2(0,0), vec2(1,0), vec2(1,1),
	// front
	vec2(1,1), vec2(0,1), vec2(0,0), vec2(1,1), vec2(0,0), vec2(1,0),
	// back
	vec2(1,1), vec2(0,1), vec2(1,0), vec2(0,0), vec2(1,0), vec2(0,1)
);

vec3 GetBlockPositionWithinChunk() {
	return vec3(
		float((v_bit_data >> 17u) & 0x1Fu),
		float((v_bit_data >> 8u)  & 0x1FFu),
		float((v_bit_data >> 3u)  & 0x1Fu)
	);
}

uint GetBlockTextureID(uint face) {
	switch (v_bit_data >> 22u) {
	case 1u: return 0u; // stone
	case 2u: return 1u; // dirt
	case 3u: return 2u; // grass
	case 4u: return 3u; // bedrock
	case 5u: return (face < 2u) ? 4u : 5u; // oak log: top/bottom vs side
	case 6u: return 6u; // leaves
	}
	return 0u;
}

void main()
{
	uint face = v_bit_data & 7u;
	int idx = int(face) * 6 + (gl_VertexID % 6);

	faceType = face;
	texID = GetBlockTextureID(face);
	texCoord = UV[idx];

	vec3 v = POS[idx];
	vec3 r = GetBlockPositionWithinChunk();

	gl_Position = v_proj * v_view * vec4(
		float(v_chunk_origin.x) + r.x + v.x,
		r.y + v.y,
		float(v_chunk_origin.y) + r.z + v.z,
		1.0);
}
