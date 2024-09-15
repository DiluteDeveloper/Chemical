#include <pch.h>

#include "block_grid.h"
#include "constants.h"

namespace Chemical {

	BlockType ChunkBlockGrid::GetBlockType(uint8_t x, unsigned int y, uint8_t z) const {
		// add x and z asserts here

		return grid[x][z][y];
	}
	void ChunkBlockGrid::SetBlockType(BlockType type, uint8_t x, unsigned int y, uint8_t z) {
		// add x and z asserts here

		//if (y >= GetGrid()[x][z].size()) {
		//	GetGrid()[x][z].resize(y + 1);

		//}
		grid[x][z][y] = type;
		if (y > max_height[x][z])
			max_height[x][z] = y;

	}

	uint16_t ChunkBlockGrid::GetMaxHeight(uint8_t x, uint8_t z) const {
		// add x and z asserts here
		return max_height[x][z];
	}

	std::array<std::array<std::array<BlockType, 50>, CHUNK_SIZE>, CHUNK_SIZE>& ChunkBlockGrid::GetGrid() {
		return grid;
	}
	const std::array<std::array<std::array<BlockType, 50>, CHUNK_SIZE>, CHUNK_SIZE>& ChunkBlockGrid::ConstGetGrid() const {
		return grid;
	}
}