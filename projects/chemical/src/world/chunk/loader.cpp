#include <pch.h>
#include <thread>
#include <mutex>
#include <future>

#include "loader.h"
#include "terrain_generator.h"
#include "util/thread_pool.h"

namespace Chemical {

	std::mutex m2;
	void Thread(ChunkGrid* grid, std::promise<std::vector<ChunkVertex>> vertices, int x, int z) {
		m2.lock();
		ChunkBlockGrid& block_grid = grid->chunk_blocks[x][z];
		ChunkBlockGrid& block_grid1 = grid->chunk_blocks[x][z - 1];
		ChunkBlockGrid& block_grid2 = grid->chunk_blocks[x][z + 1];
		ChunkBlockGrid& block_grid3 = grid->chunk_blocks[x - 1][z];
		ChunkBlockGrid& block_grid4 = grid->chunk_blocks[x + 1][z];
		m2.unlock();
		std::vector<ChunkVertex> v = BuildChunkVertices(block_grid);
		BuildChunkNorthBorderVertices(v, block_grid, block_grid1);
		BuildChunkSouthBorderVertices(v, block_grid, block_grid2);
		BuildChunkWestBorderVertices(v, block_grid, block_grid3);
		BuildChunkEastBorderVertices(v, block_grid, block_grid4);
		vertices.set_value(std::move(v));
	}

	std::unique_ptr<ChunkGrid> GenerateChunkSquare(ChunkGridOrigin starting_point, unsigned int x_size, unsigned int z_size) {
		std::unique_ptr<ChunkGrid> grid = ChunkGrid::CreateChunkGrid();

		for (int x = starting_point.x; x < x_size + starting_point.x; x++)
		{
			for (int z = starting_point.z; z < z_size + starting_point.z; z++)
			{
				ChunkHeightMap hmap = GenerateChunkHeightMapData(100, ChunkGridOrigin(x, z));

				grid->chunk_blocks[x][z] = GenerateTerrain(hmap);
				//GenerateTrees(grid->chunk_blocks[x][z]);
			}
		}

		ThreadPool pool(10);
		pool.init();
		std::vector<std::future<std::vector<ChunkVertex>>> vertices;
		for (int x = starting_point.x; x < x_size + starting_point.x; x++)
		{
			for (int z = starting_point.z; z < z_size + starting_point.z; z++)
			{
				vertices.emplace_back(pool.submit([x, z, &grid]() {
					m2.lock();
					ChunkBlockGrid& block_grid = grid->chunk_blocks[x][z];
					ChunkBlockGrid& block_grid1 = grid->chunk_blocks[x][z - 1];
					ChunkBlockGrid& block_grid2 = grid->chunk_blocks[x][z + 1];
					ChunkBlockGrid& block_grid3 = grid->chunk_blocks[x - 1][z];
					ChunkBlockGrid& block_grid4 = grid->chunk_blocks[x + 1][z];
					m2.unlock();
					std::vector<ChunkVertex> v = BuildChunkVertices(block_grid);
					BuildChunkNorthBorderVertices(v, block_grid, block_grid1);
					BuildChunkSouthBorderVertices(v, block_grid, block_grid2);
					BuildChunkWestBorderVertices(v, block_grid, block_grid3);
					BuildChunkEastBorderVertices(v, block_grid, block_grid4);
					return v;
					}));

			}
		}
		unsigned int i = 0;
		for (int x = starting_point.x; x < x_size + starting_point.x; x++)
		{
			for (int z = starting_point.z; z < z_size + starting_point.z; z++)
			{

				grid->chunk_meshes[x][z].BuildMeshFromVertices(vertices[i].get(), 1.0f);
				i++;
			}
		}
		pool.shutdown();
		return grid;
	}
}