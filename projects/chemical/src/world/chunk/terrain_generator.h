#pragma once

#include "block_grid.h"
#include "height_map.h"

namespace Chemical {
	extern ChunkBlockGrid GenerateTerrain(const ChunkHeightMap& height_map);
	extern void GenerateTrees(ChunkBlockGrid& grid);

}
