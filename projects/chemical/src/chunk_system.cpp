#include "pch.h"
#include <perlin/PerlinNoise.hpp>

#include "chunk_system.h"
#include "graphics/opengl/buffer.h"

void Chunk::GenerateHeightmap() {
	heightMap.clear();

	const siv::PerlinNoise::seed_type seed = 6232;
	const siv::PerlinNoise::seed_type seed2 = 2832;

	const siv::PerlinNoise perlin{seed};
	const siv::PerlinNoise perlin2{seed2};

	for (size_t x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (size_t z = 0; z < CHUNK_SIZE_Z; z++)
		{
			int noise = perlin.normalizedOctave2D(x * 0.03f, z * 0.03f, 4) * 20;


			heightMap.emplace_back(noise);
		}
	}
}

OpenGL::VertexArray RenderChunk(Chunk& chunk, const OpenGL::ShaderProgram& p) {
	std::vector<glm::fvec3> vertices;

	for (size_t x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (size_t z = 0; z < CHUNK_SIZE_Z; z++)
		{
			const int height = chunk.heightMap[(x * CHUNK_SIZE_Z) + z];

			vertices.emplace_back(glm::vec3(x - 0.5f, height, z - 0.5f));
			vertices.emplace_back(glm::vec3(x + 0.5f, height, z - 0.5f));
			vertices.emplace_back(glm::vec3(x - 0.5f, height, z + 0.5f));

			vertices.emplace_back(glm::vec3(x - 0.5f, height, z + 0.5f));
			vertices.emplace_back(glm::vec3(x + 0.5f, height, z - 0.5f));
			vertices.emplace_back(glm::vec3(x + 0.5f, height, z + 0.5f));
		}
	}

	std::vector<glm::vec3> vertices2 = {
		glm::vec3( - 0.5f, -0.5f, -0.5f),
		glm::vec3(0.0f, 0.5f, -0.5f),
		glm::vec3(0.5f, -0.5f, -0.5f)
	};

	OpenGL::Buffer buffer;
	LOGGER_CONSOLE_CUSTOM_MESSAGE("vertices length: {}", vertices.size());
	LOGGER_CONSOLE_CUSTOM_MESSAGE("buffer size: {}", vertices.size() * sizeof(glm::vec3));
	buffer.CreateImmutableBuffer(vertices.size() * sizeof(glm::vec3), &vertices[0]);

	OpenGL::VertexArray vArray;

	vArray.SetVertexBuffer(buffer, p.GetLayout(), 0, 0);

	return vArray;

}