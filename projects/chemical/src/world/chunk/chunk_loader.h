#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "chunk.h"

namespace Chemical {

	// sets everything up and creates a chunk square on the grid
	extern ChunkGridMap GenerateChunkSquare(ChunkGridOrigin starting_point, unsigned int x_size, unsigned int z_size);
}
