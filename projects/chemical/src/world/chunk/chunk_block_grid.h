#pragma once

#include "block.h"

namespace Chemical {

	// Chunk block grid positions start from -X -Y -Z
	class ChunkBlockGrid {
	public:

		ChunkBlockGrid();

		BlockType GetBlockType(uint8_t x = 0, unsigned int y = 0, uint8_t z = 0) const;
		void SetBlockType(BlockType type, uint8_t x = 0, unsigned int y = 0, uint8_t z = 0);

		unsigned int GetMaxHeight(uint8_t x, uint8_t z) const;

	private:
		// X, Z, Y format
		std::unique_ptr<std::array<std::array<std::vector<BlockType>, CHUNK_SIZE>, CHUNK_SIZE>> grid;

		std::array<std::array<std::vector<BlockType>, CHUNK_SIZE>, CHUNK_SIZE>& GetGrid();
		const std::array<std::array<std::vector<BlockType>, CHUNK_SIZE>, CHUNK_SIZE>& ConstGetGrid() const;
	};
}
