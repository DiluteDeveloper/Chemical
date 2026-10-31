#pragma once

#include "grid.h"

namespace Chemical {

	typedef std::array<std::array<unsigned int, CHUNK_SIZE>, CHUNK_SIZE> ChunkHeightMap;

	// origin should be in chunk coordinates
	extern ChunkHeightMap GenerateChunkHeightMapData(unsigned int seed, const ChunkOrigin& origin);

	extern ChunkBlockGrid GenerateTerrain(const ChunkHeightMap& height_map);
	extern void GenerateTrees(ChunkBlockGrid& grid);

}
