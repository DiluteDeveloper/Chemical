#pragma once

namespace Chemical {

	typedef std::array<std::array<unsigned int, CHUNK_SIZE>, CHUNK_SIZE> ChunkHeightMap;

	// origin should be in chunk coordinates
	extern ChunkHeightMap GenerateChunkHeightMapData(unsigned int seed, glm::vec2 origin);
}