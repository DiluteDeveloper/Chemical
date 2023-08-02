#include "pch.h"
#include <perlin/PerlinNoise.hpp>

#include "chunk_system.h"
#include "graphics/opengl/buffer.h"

void Chunk::GenerateHeightmap() {
	heightMap.clear();

	heightMap.reserve(CHUNK_SIZE_X * CHUNK_SIZE_Z);

	const siv::PerlinNoise perlin{std::random_device{}};
	const siv::PerlinNoise perlin2{std::random_device{}};
	const siv::PerlinNoise perlin3{std::random_device{}};

	for (size_t x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (size_t z = 0; z < CHUNK_SIZE_Z; z++)
		{
			int noise = perlin.normalizedOctave2D(x * 0.01f, z * 0.01f, 1) * 20;
			//noise += perlin2.normalizedOctave2D(x * 0.02f, z * 0.02f, 1) * 30;
			noise += perlin3.normalizedOctave2D(x * 0.004f, z * 0.004f, 1) * 80;


			heightMap.emplace_back(noise);
		}
	}
}

std::shared_ptr<ChunkRender> RenderChunk(Chunk& chunk, const OpenGL::ShaderProgram& p) {
	std::vector<glm::fvec3> vertices;

	for (size_t x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (size_t z = 0; z < CHUNK_SIZE_Z; z++)
		{
			const int index = (x * CHUNK_SIZE_Z) + z;
			const int height = chunk.heightMap[index];

			vertices.emplace_back(glm::vec3(x - 0.5f, height, z - 0.5f));
			vertices.emplace_back(glm::vec3(x + 0.5f, height, z - 0.5f));
			vertices.emplace_back(glm::vec3(x - 0.5f, height, z + 0.5f));

			vertices.emplace_back(glm::vec3(x - 0.5f, height, z + 0.5f));
			vertices.emplace_back(glm::vec3(x + 0.5f, height, z - 0.5f));
			vertices.emplace_back(glm::vec3(x + 0.5f, height, z + 0.5f));

			// X+ face

			if (x < CHUNK_SIZE_X - 1) {
				int xp_height = 0;
				xp_height = chunk.heightMap.at(index + CHUNK_SIZE_Z);


				if (xp_height != height) {
					vertices.emplace_back(glm::vec3(x + 0.5f, height, z - 0.5f));
					vertices.emplace_back(glm::vec3(x + 0.5f, xp_height, z - 0.5f));
					vertices.emplace_back(glm::vec3(x + 0.5f, height, z + 0.5f));

					vertices.emplace_back(glm::vec3(x + 0.5f, height, z + 0.5f));
					vertices.emplace_back(glm::vec3(x + 0.5f, xp_height, z - 0.5f));
					vertices.emplace_back(glm::vec3(x + 0.5f, xp_height, z + 0.5f));

				}
			}

			// Z+ face

			if (z < CHUNK_SIZE_Z - 1) {
				int zp_height = 0;
				zp_height = chunk.heightMap.at(index + 1);


				if (zp_height != height) {

					vertices.emplace_back(glm::vec3(x + 0.5f, height, z + 0.5f));
					vertices.emplace_back(glm::vec3(x - 0.5f, zp_height, z + 0.5f));
					vertices.emplace_back(glm::vec3(x - 0.5f, height, z + 0.5f));

					vertices.emplace_back(glm::vec3(x + 0.5f, zp_height, z + 0.5f));
					vertices.emplace_back(glm::vec3(x - 0.5f, zp_height, z + 0.5f));
					vertices.emplace_back(glm::vec3(x + 0.5f, height, z + 0.5f));

				}
			}

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


	std::shared_ptr<ChunkRender> render = std::make_shared<ChunkRender>();
	render->vArray.SetVertexBuffer(buffer, p.GetLayout(), 0, 0);

	render->info.count = vertices.size();
	render->info.first = 0;
	render->info.mode = OpenGL::DrawMode::TRIANGLES;

	return render;

}