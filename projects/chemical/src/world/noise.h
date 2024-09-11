#pragma once

#include <perlin/PerlinNoise.hpp>
#include <glm/glm.hpp>

#include "chunk.h"

namespace Chemical {

	// origin should be in chunk coordinates
	extern ChunkHeightMap GenerateChunkHeightMapData(unsigned int seed, glm::vec2 origin);
}