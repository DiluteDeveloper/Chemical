#pragma once

namespace Chemical {
	// a named index into the block registry in chunk_shader.
	enum BlockType:uint8_t {
		Air = 0,
		Stone = 1,
		Dirt = 2,
		Grass = 3,
		Bedrock = 4,
		OakLog = 5,
		OakLeaves = 6
	};
}
