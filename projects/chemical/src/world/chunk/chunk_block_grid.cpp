#include <pch.h>

#include "chunk_block_grid.h"

namespace Chemical {

	ChunkBlockGrid::ChunkBlockGrid() {
		grid = std::make_unique<std::array<std::array<std::vector<BlockType>, CHUNK_SIZE>, CHUNK_SIZE>>();
	}
	BlockType ChunkBlockGrid::GetBlockType(uint8_t x, unsigned int y, uint8_t z) const {
		if (y >= ConstGetGrid()[x][z].size())
			return BlockType::Air;
		else
			return ConstGetGrid()[x][z][y];
	}
	void ChunkBlockGrid::SetBlockType(BlockType type, uint8_t x, unsigned int y, uint8_t z) {
		if (y >= GetGrid()[x][z].size()) {
			GetGrid()[x][z].resize(y + 1);

		}
		GetGrid()[x][z][y] = type;

	}

	unsigned int ChunkBlockGrid::GetMaxHeight(uint8_t x, uint8_t z) const {
		return ConstGetGrid()[x][z].size() - 1;
	}

	std::array<std::array<std::vector<BlockType>, CHUNK_SIZE>, CHUNK_SIZE>& ChunkBlockGrid::GetGrid() {
		return *grid;
	}
	const std::array<std::array<std::vector<BlockType>, CHUNK_SIZE>, CHUNK_SIZE>& ChunkBlockGrid::ConstGetGrid() const {
		return *grid;
	}
}