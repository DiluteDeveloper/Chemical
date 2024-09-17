#include <pch.h>

#include "threaded.h"

namespace Chemical {

	namespace Threaded {
		ChunkBlockGrid GenerateChunkBlockGrid(std::promise<ChunkBlockGrid>&& grid, unsigned int seed, const ChunkOrigin& origin) {
			ChunkHeightMap hmap = GenerateChunkHeightMapData(seed, origin);
			ChunkBlockGrid ret_grid = GenerateTerrain(hmap);
			ChunkBlockGrid two = ret_grid;
			grid.set_value(std::move(ret_grid));
			return two;
		}

		void GenerateChunkVertices(std::promise<std::vector<ChunkVertex>>&& vertices, const ChunkBlockGrid& current, const std::shared_ptr<ChunkGrid> grid, const ChunkOrigin& origin) {

			if (grid->chunk_blocks.find(origin.x) == grid->chunk_blocks.end())
				return;
			if (grid->chunk_blocks.at(origin.x).find(origin.z - 1) == grid->chunk_blocks.at(origin.x).end())
				return;

			if (grid->chunk_blocks.find(origin.x) == grid->chunk_blocks.end())
				return;
			if (grid->chunk_blocks.at(origin.x).find(origin.z + 1) == grid->chunk_blocks.at(origin.x).end())
				return;

			if (grid->chunk_blocks.find(origin.x - 1) == grid->chunk_blocks.end())
				return;
			if (grid->chunk_blocks.at(origin.x - 1).find(origin.z) == grid->chunk_blocks.at(origin.x - 1).end())
				return;

			if (grid->chunk_blocks.find(origin.x + 1) == grid->chunk_blocks.end())
				return;
			if (grid->chunk_blocks.at(origin.x + 1).find(origin.z) == grid->chunk_blocks.at(origin.x + 1).end())
				return;

			std::vector<ChunkVertex> v = BuildChunkVertices(current);
			BuildChunkNorthBorderVertices(v, current, grid->chunk_blocks.at(origin.x).at(origin.z - 1));
			BuildChunkSouthBorderVertices(v, current, grid->chunk_blocks.at(origin.x).at(origin.z + 1));
			BuildChunkWestBorderVertices(v, current, grid->chunk_blocks.at(origin.x - 1).at(origin.z));
			BuildChunkEastBorderVertices(v, current, grid->chunk_blocks.at(origin.x + 1).at(origin.z));

			vertices.set_value(std::move(v));
		}

		void GenerateChunk(std::promise<ChunkBlockGrid>&& block_grid, std::promise<std::vector<ChunkVertex>>&& vertices,
			int seed, const ChunkOrigin& origin, const std::shared_ptr<ChunkGrid> grid) {

			ChunkHeightMap hmap = GenerateChunkHeightMapData(seed, origin);
			ChunkBlockGrid ret_grid = GenerateTerrain(hmap);

			std::vector<ChunkVertex> v = BuildChunkVertices(ret_grid);

			if (grid->chunk_blocks.find(origin.x) != grid->chunk_blocks.end())
				if(
				grid->chunk_blocks.at(origin.x).find(origin.z - 1) != grid->chunk_blocks.at(origin.x).end())
				BuildChunkNorthBorderVertices(v, ret_grid, grid->chunk_blocks.at(origin.x).at(origin.z - 1));

			if (grid->chunk_blocks.find(origin.x) != grid->chunk_blocks.end())
				if (
				grid->chunk_blocks.at(origin.x).find(origin.z + 1) != grid->chunk_blocks.at(origin.x).end())
				BuildChunkSouthBorderVertices(v, ret_grid, grid->chunk_blocks.at(origin.x).at(origin.z + 1));

			if (grid->chunk_blocks.find(origin.x - 1) != grid->chunk_blocks.end())
				if (grid->chunk_blocks.at(origin.x - 1).find(origin.z) != grid->chunk_blocks.at(origin.x - 1).end())
					BuildChunkWestBorderVertices(v, ret_grid, grid->chunk_blocks.at(origin.x - 1).at(origin.z));

			if (grid->chunk_blocks.find(origin.x + 1) != grid->chunk_blocks.end())
				if (
				grid->chunk_blocks.at(origin.x + 1).find(origin.z) != grid->chunk_blocks.at(origin.x - 1).end())
				BuildChunkEastBorderVertices(v, ret_grid, grid->chunk_blocks.at(origin.x + 1).at(origin.z));

			vertices.set_value(std::move(v));
			block_grid.set_value(std::move(ret_grid));

		}
	}
}