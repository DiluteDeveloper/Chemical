#pragma once

#include "chunk.h"

namespace Chemical {

	// Chunk origin coordinates in chunk space
	// aka x * CHUNK_SIZE & z * CHUNK_SIZE
	struct ChunkGridOrigin {
		int x = 0;
		int z = 0;

		ChunkGridOrigin(int x = 0, int z = 0) :
			x(x), z(z) {}
	};

	class ChunkGridMap {
	public:
		// returns nullptr if chunk doesnt exist at origin
		Chunk const* GetChunk(ChunkGridOrigin origin) const;

		// returns existing chunk if already exists
		Chunk& CreateChunk(ChunkGridOrigin origin);

		std::unordered_map<int, std::unordered_map<int, Chunk>>& GetGridMap();

	private:
		std::unordered_map<int, std::unordered_map<int, Chunk>> grid_map;
	};
}