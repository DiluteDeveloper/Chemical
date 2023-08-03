#include "pch.h"
#include <perlin/PerlinNoise.hpp>

#include <stdlib.h>

#include "chunk_system.h"
#include "graphics/opengl/buffer.h"

Chunk::Chunk(const glm::dvec3& origin) :
	origin(origin) {

	const siv::PerlinNoise perlin{std::random_device{}};
	const siv::PerlinNoise perlin2{std::random_device{}};
	const siv::PerlinNoise perlin3{std::random_device{}};

	for (size_t x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (size_t z = 0; z < CHUNK_SIZE_Z; z++)
		{
			int genHeight = perlin.normalizedOctave2D(x * 0.01f, z * 0.01f, 1) * 20;
			genHeight += perlin2.normalizedOctave2D(x * 0.02f, z * 0.02f, 4) * 30;
			genHeight += perlin3.normalizedOctave2D(x * 0.004f, z * 0.004f, 1) * 80;

			visibleBlocks[x][z].emplace_back(genHeight); // generate the top block

			// Since the X+ and Z+ blocks arent generated yet, we need to go backwards compared to the rendering.
			if (x > 0) {

				// index 0 will always be the top block
				// Calculates the height difference between current block and block x - 1
				int16_t heightDiff = visibleBlocks[x][z][0] - visibleBlocks[x - 1][z][0];

				if(heightDiff < 0) // if it is equal to 0, the positive branch will not run either
					for (int16_t i = -1; i > heightDiff; i--)
					{
						//LOGGER_CONSOLE_CUSTOM_MESSAGE("LOWER HEIGHT TOP X: {}, Y: {}, Z: {}", x, genHeight, z);
						//LOGGER_CONSOLE_CUSTOM_MESSAGE("LOWER HEIGHT BOTTOM X: {}, Y: {}, Z: {}", x, genHeight + i, z);
						visibleBlocks[x][z].emplace_back(genHeight + i);
					}
				else
					for (size_t i = 1; i < heightDiff; i++)
					{
						//LOGGER_CONSOLE_CUSTOM_MESSAGE("GREATER HEIGHT TOP X: {}, Y: {}, Z: {}", x, genHeight, z);
						//LOGGER_CONSOLE_CUSTOM_MESSAGE("GREATER HEIGHT BOTTOM X: {}, Y: {}, Z: {}", x, genHeight + i, z);
						visibleBlocks[x][z].emplace_back(genHeight + i);
					}

			}
		}
	}
}

std::shared_ptr<ChunkRender> RenderChunk(Chunk& chunk, const OpenGL::ShaderProgram& p) {
	std::vector<ChunkVertex> vertices;

	for (size_t x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (size_t z = 0; z < CHUNK_SIZE_Z; z++)
		{
			const int16_t topBlockY = chunk.visibleBlocks[x][z][0];

			vertices.emplace_back(glm::vec3(x - 0.5f, topBlockY, z - 0.5f), glm::vec3(0, 1, 0));
			vertices.emplace_back(glm::vec3(x + 0.5f, topBlockY, z - 0.5f), glm::vec3(0, 1, 0));
			vertices.emplace_back(glm::vec3(x - 0.5f, topBlockY, z + 0.5f), glm::vec3(0, 1, 0));

			vertices.emplace_back(glm::vec3(x - 0.5f, topBlockY, z + 0.5f), glm::vec3(0, 1, 0));
			vertices.emplace_back(glm::vec3(x + 0.5f, topBlockY, z - 0.5f), glm::vec3(0, 1, 0));
			vertices.emplace_back(glm::vec3(x + 0.5f, topBlockY, z + 0.5f), glm::vec3(0, 1, 0));

			// X+ face

			if (x < CHUNK_SIZE_X - 1) {
				int16_t xp_height = chunk.visibleBlocks[x + 1][z][0];


				if (xp_height != topBlockY) {
					vertices.emplace_back(glm::vec3(x + 0.5f, topBlockY, z - 0.5f), glm::vec3(1, 0, 0));
					vertices.emplace_back(glm::vec3(x + 0.5f, xp_height, z - 0.5f), glm::vec3(1, 0, 0));
					vertices.emplace_back(glm::vec3(x + 0.5f, topBlockY, z + 0.5f), glm::vec3(1, 0, 0));

					vertices.emplace_back(glm::vec3(x + 0.5f, topBlockY, z + 0.5f), glm::vec3(1, 0, 0));
					vertices.emplace_back(glm::vec3(x + 0.5f, xp_height, z - 0.5f), glm::vec3(1, 0, 0));
					vertices.emplace_back(glm::vec3(x + 0.5f, xp_height, z + 0.5f), glm::vec3(1, 0, 0));

				}
			}

			// Z+ face

			if (z < CHUNK_SIZE_Z - 1) {
				int16_t zp_height = chunk.visibleBlocks[x][z + 1][0];


				if (zp_height != topBlockY) {

					vertices.emplace_back(glm::vec3(x + 0.5f, topBlockY, z + 0.5f), glm::vec3(0, 0, 1));
					vertices.emplace_back(glm::vec3(x - 0.5f, zp_height, z + 0.5f), glm::vec3(0, 0, 1));
					vertices.emplace_back(glm::vec3(x - 0.5f, topBlockY, z + 0.5f), glm::vec3(0, 0, 1));

					vertices.emplace_back(glm::vec3(x + 0.5f, zp_height, z + 0.5f), glm::vec3(0, 0, 1));
					vertices.emplace_back(glm::vec3(x - 0.5f, zp_height, z + 0.5f), glm::vec3(0, 0, 1));
					vertices.emplace_back(glm::vec3(x + 0.5f, topBlockY, z + 0.5f), glm::vec3(0, 0, 1));

				}
			}

		}
	}

	OpenGL::Buffer buffer;
	buffer.CreateImmutableBuffer(vertices.size() * sizeof(ChunkVertex), &vertices[0]);


	std::shared_ptr<ChunkRender> render = std::make_shared<ChunkRender>();
	render->vArray.SetVertexBuffer(buffer, p.GetLayout(), 0, 0);

	render->info.count = vertices.size();
	render->info.first = 0;
	render->info.mode = OpenGL::DrawMode::TRIANGLES;

	return render;

}