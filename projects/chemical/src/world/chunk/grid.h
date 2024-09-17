#pragma once

#include "../block.h"
#include "block_grid.h"
#include "mesh.h"

namespace Chemical {

	// Chunk origin coordinates in chunk space
	// aka x * CHUNK_SIZE & z * CHUNK_SIZE
	struct ChunkOrigin {
		int x = 0;
		int z = 0;

		ChunkOrigin(int x = 0, int z = 0) :
			x(x), z(z) {}

		ChunkOrigin operator-(const ChunkOrigin& rhs) const {
			return ChunkOrigin(x - rhs.x, z - rhs.z);
		}
		bool operator !=(const ChunkOrigin& rhs) const {
			return x != rhs.x && z != rhs.z;
		}
	};
	struct ChunkGrid {
	public:


		std::unordered_map<int, std::unordered_map<int, ChunkBlockGrid>> chunk_blocks;
		std::unordered_map<int, std::unordered_map<int, ChunkMesh>> chunk_meshes;

	private:


	};
}