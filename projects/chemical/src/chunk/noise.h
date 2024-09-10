#pragma once

#include <perlin/PerlinNoise.hpp>
#include <glm/glm.hpp>

#include "chunk.h"

namespace Chemical {

	// origin should be in chunk coordinates
	inline ChunkHeightMapData GenerateChunkHeightMapData(const siv::PerlinNoise& noise_gen, glm::vec2 origin) {
		ChunkHeightMapData height_map;
		for (uint8_t x = 0; x < CHUNK_SIZE; x++)
		{
			for (uint8_t z = 0; z < CHUNK_SIZE; z++)
			{
				height_map[x][z] = static_cast<int16_t>((
					noise_gen.octave2D_01(((origin.x * CHUNK_SIZE) + x) * 0.05f, ((origin.y * CHUNK_SIZE) + z) * 0.05f, 1) * 20) + 1);
			}
		}
		return height_map;
	}
}