#pragma once

#include "../block.h"
#include "block_grid.h"
#include "mesh.h"

namespace Chemical {

	// Chunk origin coordinates in chunk space
	// aka x * CHUNK_SIZE & z * CHUNK_SIZE
	struct ChunkGridOrigin {
		int x = 0;
		int z = 0;

		ChunkGridOrigin(int x = 0, int z = 0) :
			x(x), z(z) {}
	};
	struct ChunkGrid {
	public:
		friend std::unique_ptr<ChunkGrid> std::make_unique<ChunkGrid>();
		static std::unique_ptr<ChunkGrid> CreateChunkGrid() {
			return std::make_unique<ChunkGrid>();
		}


		std::unordered_map<int, std::unordered_map<int, ChunkBlockGrid>> chunk_blocks;
		std::unordered_map<int, std::unordered_map<int, ChunkMesh>> chunk_meshes;

	private:
		ChunkGrid() = default;

	};
}