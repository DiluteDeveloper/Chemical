#include <pch.h>

#include "chunk_loader.h"
#include "noise.h"

namespace Chemical {
	ChunkGridMap GenerateChunkSquare(ChunkGridOrigin starting_point, unsigned int x_size, unsigned int z_size) {
		ChunkGridMap grid_map;

		for (int x = starting_point.x; x < x_size + starting_point.x; x++)
		{
			for (int z = starting_point.z; z < z_size + starting_point.z; z++)
			{
				Chunk& chunk = grid_map.CreateChunk(ChunkGridOrigin{ x, z });
				ChunkHeightMap height_map = GenerateChunkHeightMapData(100, glm::vec2(x, z));
				chunk.GenerateTerrainFromHeightMap(height_map);
			}
		}
		auto& chunks = grid_map.GetGridMap();

		for (auto& [x, map] : chunks) {
			for (auto& [z, chunk] : map) {
				std::vector<ChunkVertex> vertices = chunk.BuildVertices();
				chunk.BuildChunkBorderVertices(vertices, grid_map);

				chunk.BuildMeshFromVertices(vertices, 1.0f);

			}
		}

		return grid_map;
	}
}