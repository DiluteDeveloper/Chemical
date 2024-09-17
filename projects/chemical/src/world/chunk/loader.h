#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <future>

#include "grid.h"

namespace Chemical {

	extern void GenerateChunk(std::shared_ptr<ChunkGrid> grid, const ChunkOrigin& origin, unsigned int seed);
	extern void GenerateChunkFullMesh(std::shared_ptr<ChunkGrid> grid, const ChunkOrigin& origin, unsigned int seed);

	// sets everything up and creates a chunk square on the grid
	extern std::shared_ptr<ChunkGrid> GenerateChunkSquare(ChunkOrigin starting_point, unsigned int x_size, unsigned int z_size);

	class ChunkLoader {
	public:
		ChunkLoader(const glm::vec3& center_pos, unsigned int seed, uint16_t render_distance);

		void UpdateCenter(const glm::vec3& center_pos);

		std::shared_ptr<ChunkGrid> GetChunkGrid();
	private:
		ChunkOrigin center;

		std::future<ChunkBlockGrid> future_blocks;
		std::future<std::vector<ChunkVertex>> future_vertices;
		std::shared_ptr<ChunkGrid> grid;

		uint16_t render_distance = 0;
		unsigned int seed = 0;
	};
}
