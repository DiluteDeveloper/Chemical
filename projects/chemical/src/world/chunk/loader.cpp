#include <pch.h>
#include <thread>
#include <mutex>
#include <future>

#include "loader.h"
#include "height_map.h"

#include "util/thread_pool.h"
#include "threaded.h"

namespace Chemical {

	void GenerateChunk(std::shared_ptr<ChunkGrid> grid, const ChunkOrigin& origin, unsigned int seed) {
		//ChunkHeightMap hmap = GenerateChunkHeightMapData(seed, origin);

		//grid->chunk_blocks[origin.x][origin.z] = GenerateTerrain(hmap);
	}

	void GenerateChunkFullMesh(std::shared_ptr<ChunkGrid> grid, const ChunkOrigin & origin, unsigned int seed) {
		//std::vector<ChunkVertex> v = BuildChunkVertices(grid->chunk_blocks[origin.x][origin.z]);
		//BuildChunkNorthBorderVertices(v, grid->chunk_blocks[origin.x][origin.z], grid->chunk_blocks[origin.x][origin.z - 1]);
		//BuildChunkSouthBorderVertices(v, grid->chunk_blocks[origin.x][origin.z], grid->chunk_blocks[origin.x][origin.z + 1]);
		//BuildChunkWestBorderVertices(v, grid->chunk_blocks[origin.x][origin.z], grid->chunk_blocks[origin.x - 1][origin.z]);
		//BuildChunkEastBorderVertices(v, grid->chunk_blocks[origin.x][origin.z], grid->chunk_blocks[origin.x + 1][origin.z]);
		//grid->chunk_meshes[origin.x][origin.z].BuildMeshFromVertices(v, 1.0f);
		
	}
	std::mutex m2;
	std::shared_ptr<ChunkGrid> GenerateChunkSquare(ChunkOrigin starting_point, unsigned int x_size, unsigned int z_size) {
		std::shared_ptr<ChunkGrid> grid = std::make_shared<ChunkGrid>();

		/*for (int x = starting_point.x; x < (int)x_size + starting_point.x; x++)
		{
			for (int z = starting_point.z; z < (int)z_size + starting_point.z; z++)
			{
				ChunkHeightMap hmap = GenerateChunkHeightMapData(100, ChunkOrigin(x, z));

				grid->chunk_blocks[x][z] = GenerateTerrain(hmap);
				//GenerateTrees(grid->chunk_blocks[x][z]);
			}
		}

		ThreadPool pool(10);
		pool.init();
		std::vector<std::future<std::vector<ChunkVertex>>> vertices;
		for (int x = starting_point.x; x < (int)x_size + starting_point.x; x++)
		{
			for (int z = starting_point.z; z < (int)z_size + starting_point.z; z++)
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
		for (int x = starting_point.x; x < (int)x_size + starting_point.x; x++)
		{
			for (int z = starting_point.z; z < (int)z_size + starting_point.z; z++)
			{

				//grid->chunk_meshes[x][z].BuildMeshFromVertices(vertices[i].get(), 1.0f);
				i++;
			}
		}
		pool.shutdown();*/
		return grid;
	}
	void MThread(std::promise<std::vector<std::vector<ChunkVertex>>>&& v, std::promise<ChunkGrid*>&& p, uint16_t render_distance, unsigned int seed, const glm::vec3& center_pos) {
		/*ChunkGrid* g = new ChunkGrid();
		for (int x = -render_distance; x < render_distance + 1; x++)
		{
			for (int z = -render_distance; z < render_distance + 1; z++)
			{
				ChunkOrigin origin(x, z);
				ChunkHeightMap hmap = GenerateChunkHeightMapData(seed, origin);
				g->chunk_blocks[x][z] = GenerateTerrain(hmap);
			}

		}

		ThreadPool pool(10);
		pool.init();
		std::vector<std::future<std::vector<ChunkVertex>>> vertices;
		for (int x = -render_distance; x < render_distance + 1; x++)
		{
			for (int z = -render_distance; z < render_distance + 1; z++)
			{
				vertices.emplace_back(pool.submit([g, x, z]() {
					m2.lock();
					ChunkBlockGrid& block_grid = g->chunk_blocks[x][z];
					ChunkBlockGrid& block_grid1 = g->chunk_blocks[x][z - 1];
					ChunkBlockGrid& block_grid2 = g->chunk_blocks[x][z + 1];
					ChunkBlockGrid& block_grid3 = g->chunk_blocks[x - 1][z];
					ChunkBlockGrid& block_grid4 = g->chunk_blocks[x + 1][z];
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
		std::vector<std::vector<ChunkVertex>> n_vertices;
		unsigned int i = 0;
		for (int x = -render_distance; x < render_distance + 1; x++)
		{
			for (int z = -render_distance; z < render_distance + 1; z++)
			{
				n_vertices.emplace_back(vertices[i].get());
				i++;
			}
		}
		pool.shutdown();
		p.set_value(std::move(g));
		v.set_value(std::move(n_vertices));*/
	}
	void GThread(std::promise<ChunkGrid>&& grid, uint16_t render_distance, unsigned int seed, ChunkOrigin& center, const glm::vec3& center_pos) {

		/*
		glm::ivec2 new_center(static_cast<int>(std::floor(center_pos.x / CHUNK_SIZE)),
	static_cast<int>(std::floor(center_pos.z / CHUNK_SIZE)));

	glm::ivec2 old_center = glm::ivec2(center.x, center.z);

	if (new_center != old_center) {
		// player has moved into the bounding region of another chunk

		// get which direction the player went in
		glm::ivec2 offset = new_center - old_center;

		if (offset.y != 0) {
			// the player changed Z chunk

			int32_t z_origin = old_center.y + ((render_distance + 1) * offset.y);

			for (int16_t x = -render_distance; x < render_distance + 1; x++)
			{
				int32_t x_origin = old_center.x + x;
				int32_t neg_z_origin = old_center.y + (render_distance * -offset.y);

				grid->chunk_meshes[x_origin].erase(neg_z_origin);

				GenerateChunk(grid, ChunkOrigin(x_origin, z_origin), seed);
				GenerateChunkFullMesh(grid, ChunkOrigin(x_origin, z_origin), seed);

			}

			center.z = new_center.y;
		}
		if (offset.x != 0) {
			// the player changed X chunk

			int32_t neg_x_origin = old_center.x + (render_distance * -offset.x);
			int32_t x_origin = old_center.x + ((render_distance + 1) * offset.x);

			for (int16_t z = -render_distance; z < render_distance + 1; z++)
			{

				int32_t z_origin = old_center.y + z;

				grid->chunk_meshes[neg_x_origin].erase(z_origin);

				GenerateChunk(grid, ChunkOrigin(x_origin, z_origin), seed);
				GenerateChunkFullMesh(grid, ChunkOrigin(x_origin, z_origin), seed);
			}

			grid->chunk_meshes.erase(neg_x_origin);
			center.x = new_center.x;
		}
	}*/
	}

	int x = 0;
	int z = 0;

	ChunkLoader::ChunkLoader(const glm::vec3& center_pos, unsigned int seed, uint16_t render_distance) :
	render_distance(render_distance), seed(seed) {
		grid = std::make_shared<ChunkGrid>();

		x = -render_distance;
		z = -render_distance;

		std::promise<ChunkBlockGrid> prm_blocks;
		std::promise<std::vector<ChunkVertex>> prm_vertices;
		future_blocks = prm_blocks.get_future();
		future_vertices = prm_vertices.get_future();

		std::thread th(Threaded::GenerateChunk, std::move(prm_blocks), std::move(prm_vertices), seed, ChunkOrigin{ x, z }, grid);

		th.detach();


		
	}
	int it = 0;
	void ChunkLoader::UpdateCenter(const glm::vec3& center_pos) {
		if (future_blocks._Is_ready() && future_vertices._Is_ready()) {
			grid->chunk_blocks[x][z] = future_blocks.get();
			grid->chunk_meshes[x][z].BuildMeshFromVertices(future_vertices.get(), 1.0f);

			if (x < render_distance + 1) {
				here:
				if (z < render_distance + 1) {

					std::promise<ChunkBlockGrid> prm_blocks;
					std::promise<std::vector<ChunkVertex>> prm_vertices;
					future_blocks = prm_blocks.get_future();
					future_vertices = prm_vertices.get_future();
					std::cout << "X: " << x << ", Z: " << z << std::endl;
					std::thread th(Threaded::GenerateChunk, std::move(prm_blocks), std::move(prm_vertices), seed, ChunkOrigin{ x, z }, grid);
					th.detach();

					z++;
				}
				else {
					z = -render_distance;
					x++;
					goto here;
				}

			}
		}
		/*glm::ivec2 new_center(static_cast<int>(std::floor(center_pos.x / CHUNK_SIZE)),
			static_cast<int>(std::floor(center_pos.z / CHUNK_SIZE)));
		glm::ivec2 old_center = glm::ivec2(center.x, center.z);

		if (new_center != old_center) {
			// player has moved into the bounding region of another chunk

			// get which direction the player went in
			glm::ivec2 offset = new_center - old_center;

			if (offset.y != 0) {
				// the player changed Z chunk

				int32_t z_origin = old_center.y + ((render_distance + 1) * offset.y);

				for (int16_t x = -render_distance; x < render_distance + 1; x++)
				{
					int32_t x_origin = old_center.x + x;
					int32_t neg_z_origin = old_center.y + (render_distance * -offset.y);

					grid->chunk_meshes[x_origin].erase(neg_z_origin);

					GenerateChunk(grid, ChunkOrigin(x_origin, z_origin), seed);
					GenerateChunkFullMesh(grid, ChunkOrigin(x_origin, z_origin), seed);

				}

				center.z = new_center.y;
			}
			if (offset.x != 0) {
				// the player changed X chunk

				int32_t neg_x_origin = old_center.x + (render_distance * -offset.x);
				int32_t x_origin = old_center.x + ((render_distance + 1) * offset.x);

				for (int16_t z = -render_distance; z < render_distance + 1; z++)
				{

					int32_t z_origin = old_center.y + z;

					grid->chunk_meshes[neg_x_origin].erase(z_origin);

					GenerateChunk(grid, ChunkOrigin(x_origin, z_origin), seed);
					GenerateChunkFullMesh(grid, ChunkOrigin(x_origin, z_origin), seed);
				}

				grid->chunk_meshes.erase(neg_x_origin);
				center.x = new_center.x;
			}
		}*/
	}

	std::shared_ptr<ChunkGrid> ChunkLoader::GetChunkGrid() {
		return grid;
	}
}