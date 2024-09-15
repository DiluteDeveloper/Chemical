#include <pch.h>
#include <random>

#include "terrain_generator.h"

namespace Chemical {
	ChunkBlockGrid GenerateTerrain(const ChunkHeightMap& height_map) {
		ChunkBlockGrid grid;
		for (uint8_t x = 0; x < CHUNK_SIZE; x++)
		{
			for (uint8_t z = 0; z < CHUNK_SIZE; z++)
			{
				unsigned int height = height_map[x][z];

				// reverse iterator to go from terrain height - 0(bottom y of the chunk)
				// If genHeight is past CHUNK_HEIGHT it will be empty past that height
				for (int64_t y = height; y >= 0; y--)
				{

					if (y == 0) {
						grid.SetBlockType(BlockType::Bedrock, x, y, z);
						continue;
					}


					if (y <= height - 1) {
						if (y <= height - 4)
							grid.SetBlockType(BlockType::Stone, x, y, z);
						else
							grid.SetBlockType(BlockType::Dirt, x, y, z);
					}
					else
						grid.SetBlockType(BlockType::Grass, x, y, z);
				}
			}
		}

		return grid;
	}

	extern void GenerateTrees(ChunkBlockGrid& grid) {
		std::random_device rd; // obtain a random number from hardware
		std::mt19937 gen(rd()); // seed the generator
		std::uniform_int_distribution<> distr(-100, 100);
		for (uint8_t x = 0; x < CHUNK_SIZE; x++)
		{
			for (uint8_t z = 0; z < CHUNK_SIZE; z++)
			{
				unsigned int max_height = grid.GetMaxHeight(x, z);
				if (distr(gen) == 0) {
					grid.SetBlockType(BlockType::OakLog, x, max_height + 1, z);
					grid.SetBlockType(BlockType::OakLog, x, max_height + 2, z);
					grid.SetBlockType(BlockType::OakLog, x, max_height + 3, z);
					grid.SetBlockType(BlockType::OakLog, x, max_height + 4, z);
					grid.SetBlockType(BlockType::OakLog, x, max_height + 5, z);
				}

			}
		}
	}
}