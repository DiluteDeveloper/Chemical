#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "grid.h"

namespace Chemical {

	// sets everything up and creates a chunk square on the grid
	extern std::unique_ptr<ChunkGrid> GenerateChunkSquare(ChunkGridOrigin starting_point, unsigned int x_size, unsigned int z_size);
}
