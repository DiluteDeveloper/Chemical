#pragma once

#include <future>

#include "block_grid.h"
#include "height_map.h"

namespace Chemical {

	namespace Threaded {
		extern ChunkBlockGrid GenerateChunkBlockGrid(std::promise<ChunkBlockGrid>&& grid, unsigned int seed, const ChunkOrigin& origin);

		extern void GenerateChunkVertices(std::promise<std::vector<ChunkVertex>>&& vertices, const ChunkBlockGrid& current, const std::shared_ptr<ChunkGrid> grid, const ChunkOrigin& origin);

		extern void GenerateChunk(std::promise<ChunkBlockGrid>&& block_grid, std::promise<std::vector<ChunkVertex>>&& vertices,
			int seed, const ChunkOrigin& origin, const std::shared_ptr<ChunkGrid> grid);
	}

}
