#pragma once

#include "constants.h"
#include "grid.h"

namespace Chemical {

	typedef std::array<std::array<unsigned int, CHUNK_SIZE>, CHUNK_SIZE> ChunkHeightMap;

	// origin should be in chunk coordinates
	extern ChunkHeightMap GenerateChunkHeightMapData(unsigned int seed, const ChunkGridOrigin& origin);
}