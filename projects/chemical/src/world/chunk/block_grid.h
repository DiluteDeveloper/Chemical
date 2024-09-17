#pragma once

#include "../block.h"
#include "constants.h"

namespace Chemical {

	// Chunk block grid positions start from -X -Y -Z
	// Makes the block grid data structure easier to use
	class ChunkBlockGrid {
	public:

		BlockType GetBlockType(uint8_t x = 0, unsigned int y = 0, uint8_t z = 0) const;
		void SetBlockType(BlockType type, uint8_t x = 0, unsigned int y = 0, uint8_t z = 0);

		uint16_t GetMaxHeight(uint8_t x, uint8_t z) const;

	private:
		// X, Z, Y format
		std::array<std::array<std::array<BlockType, HEIGHT_LIMIT>, CHUNK_SIZE>, CHUNK_SIZE> grid = {};
		std::array<std::array<uint16_t, CHUNK_SIZE>, CHUNK_SIZE> max_height = {};
	};
}
