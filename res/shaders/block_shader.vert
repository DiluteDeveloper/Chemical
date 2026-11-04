#version 300 es

layout(location = 0) in ivec3 block_position;
layout(location = 1) in uint block_data;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

flat out uint faceType;
out vec2 texCoord;

const uvec3 POS[36] = uvec3[36](
	// top
	uvec3(0,1000,0), uvec3(1000,1000,0), uvec3(0,1000,1000), uvec3(0,1000,1000), uvec3(1000,1000,0), uvec3(1000,1000,1000),
	// bottom
	uvec3(0,0,1000), uvec3(1000,0,0), uvec3(0,0,0), uvec3(1000,0,1000), uvec3(1000,0,0), uvec3(0,0,1000),
	// right
	uvec3(1000,0,0), uvec3(1000,0,1000), uvec3(1000,1000,0), uvec3(1000,0,1000), uvec3(1000,1000,1000), uvec3(1000,1000,0),
	// left
	uvec3(0,1000,0), uvec3(0,0,1000), uvec3(0,0,0), uvec3(0,1000,0), uvec3(0,1000,1000), uvec3(0,0,1000),
	// front
	uvec3(1000,0,1000), uvec3(0,0,1000), uvec3(0,1000,1000), uvec3(1000,0,1000), uvec3(0,1000,1000), uvec3(1000,1000,1000),
	// back
	uvec3(0,0,0), uvec3(1000,0,0), uvec3(0,1000,0), uvec3(1000,1000,0), uvec3(0,1000,0), uvec3(1000,0,0)
);

const uvec2 UV[36] = uvec2[36](
	// top
	uvec2(0,1), uvec2(1,1), uvec2(0,0), uvec2(0,0), uvec2(1,1), uvec2(1,0),
	// bottom
	uvec2(0,1), uvec2(1,0), uvec2(0,0), uvec2(1,1), uvec2(1,0), uvec2(0,1),
	// right
	uvec2(0,1), uvec2(1,1), uvec2(0,0), uvec2(1,1), uvec2(1,0), uvec2(0,0),
	// left
	uvec2(0,0), uvec2(1,1), uvec2(0,1), uvec2(0,0), uvec2(1,0), uvec2(1,1),
	// front
	uvec2(1,1), uvec2(0,1), uvec2(0,0), uvec2(1,1), uvec2(0,0), uvec2(1,0),
	// back
	uvec2(1,1), uvec2(0,1), uvec2(1,0), uvec2(0,0), uvec2(1,0), uvec2(0,1)
);

void main()
{
	uint face = block_data & 7u;
	int idx = int(face) * 6 + (gl_InstanceID % 6);

	faceType = face;
	texCoord = vec2(UV[idx]);

	uvec3 v = POS[idx];

    // 1016 == 0b1111111000
    // v.z *= block_data & uint(1016);

    // 130048 == 0b11111110000000000
    // v.y *= block_data & uint(130048);

    // 16646144 == 0b11111110000000000000000
    // v.x *= block_data & uint(16646144);

    // 1016 == 0b1111111000
    // v.y *= block_data & uint(1 << (3 + 7));
    // v.z *= block_data & uint(1 << 3);

	gl_Position = projection_matrix * view_matrix * vec4(
		block_position.x + int(v.x),
		block_position.y + int(v.y),
		block_position.z + int(v.z),
		1.0);
}
