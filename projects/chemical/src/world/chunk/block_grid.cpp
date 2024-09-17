#include <pch.h>

#include "block_grid.h"
#include "constants.h"

namespace Chemical {

	BlockType ChunkBlockGrid::GetBlockType(uint8_t x, unsigned int y, uint8_t z) const {
		assert(x < CHUNK_SIZE && z < CHUNK_SIZE && y < HEIGHT_LIMIT);

		return grid[x][z][y];
	}
	void ChunkBlockGrid::SetBlockType(BlockType type, uint8_t x, unsigned int y, uint8_t z) {
		assert(x < CHUNK_SIZE && z < CHUNK_SIZE && y < HEIGHT_LIMIT);

		grid[x][z][y] = type;
		if (y > max_height[x][z])
			max_height[x][z] = y;

	}

	uint16_t ChunkBlockGrid::GetMaxHeight(uint8_t x, uint8_t z) const {
		assert(x < CHUNK_SIZE && z < CHUNK_SIZE);

		return max_height[x][z];
	}
}