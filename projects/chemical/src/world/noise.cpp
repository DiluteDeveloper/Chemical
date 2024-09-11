#include <pch.h>

#include "noise.h"

namespace Chemical {

	int16_t GenerateNoiseLayer(const siv::PerlinNoise& noise_gen, glm::vec2 position, float scale, unsigned int amplitude) {
		return static_cast<int16_t>(noise_gen.octave2D_01(position.x * scale, position.y * scale, 1) * amplitude);
	}

	ChunkHeightMap GenerateChunkHeightMapData(unsigned int seed, glm::vec2 origin) {
		ChunkHeightMap height_map;
		const siv::PerlinNoise noise_gen{ seed };
		for (uint8_t x = 0; x < CHUNK_SIZE; x++)
		{
			for (uint8_t z = 0; z < CHUNK_SIZE; z++)
			{
				height_map[x][z] = GenerateNoiseLayer(noise_gen, glm::vec2((origin.x * CHUNK_SIZE) + x, (origin.y * CHUNK_SIZE) + z), 0.05f, 50);
				height_map[x][z] += GenerateNoiseLayer(noise_gen, glm::vec2((origin.x * CHUNK_SIZE) + x, (origin.y * CHUNK_SIZE) + z), 0.005f, 50);
			}
		}
		return height_map;
	}
}