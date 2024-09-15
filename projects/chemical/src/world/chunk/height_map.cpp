#include <pch.h>

#include <perlin/PerlinNoise.hpp>

#include "height_map.h"

namespace Chemical {

	unsigned int GenerateNoiseLayer(const siv::PerlinNoise& noise_gen, glm::vec2 position, float scale, unsigned int amplitude) {
		return static_cast<unsigned int>(noise_gen.octave2D_01(position.x * scale, position.y * scale, 1) * amplitude);
	}

	ChunkHeightMap GenerateChunkHeightMapData(unsigned int seed, const ChunkGridOrigin& origin) {
		ChunkHeightMap height_map;
		const siv::PerlinNoise noise_gen{ seed };
		for (uint8_t x = 0; x < CHUNK_SIZE; x++)
		{
			for (uint8_t z = 0; z < CHUNK_SIZE; z++)
			{
				height_map[x][z] = GenerateNoiseLayer(noise_gen, glm::vec2((origin.x * CHUNK_SIZE) + x, (origin.z * CHUNK_SIZE) + z), 0.03f, 50);
			}
		}
		return height_map;
	}
}

