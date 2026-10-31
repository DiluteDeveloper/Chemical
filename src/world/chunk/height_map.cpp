#include <random>
#include <algorithm>
#include <glm/glm.hpp>

#define STB_PERLIN_IMPLEMENTATION
#include <stb_perlin.h>

#include "height_map.h"

namespace Chemical {

    unsigned int GenerateNoiseLayer(int seed, glm::vec2 position, float scale, unsigned int amplitude) {
        // noise3 is 0 on integer lattice planes, so sample a fixed non-integer z slice for 2D noise.
        // stb returns about -1..1, so remap to 0..1 like octave2D_01 did.
        const float n = stb_perlin_noise3_seed(position.x * scale, position.y * scale, 0.5f, 0, 0, 0, seed);
        const float n01 = std::clamp(n * 0.5f + 0.5f, 0.0f, 1.0f);
        return static_cast<unsigned int>(n01 * amplitude);
    }

    ChunkHeightMap GenerateChunkHeightMapData(unsigned int seed, const ChunkOrigin& origin) {
        ChunkHeightMap height_map;
        const int stb_seed = static_cast<int>(seed & 255); // stb only uses the low 8 bits
        for (uint8_t x = 0; x < CHUNK_SIZE; x++)
        {
            for (uint8_t z = 0; z < CHUNK_SIZE; z++)
            {
                height_map[x][z] = GenerateNoiseLayer(stb_seed, glm::vec2((origin.x * CHUNK_SIZE) + x, (origin.z * CHUNK_SIZE) + z), 0.07f, 60);
            }
        }
        return height_map;
    }

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
						if (y <= height - 4) {
							grid.SetBlockType(BlockType::Stone, x, y, z);
                        }

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
