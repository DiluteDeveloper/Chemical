#include "pch.h"
#include <perlin/PerlinNoise.hpp>

#include <stdlib.h>

#include "chunk_system.h"
#include "graphics/opengl/buffer.h"


namespace Chemical {


	Chunk::Chunk(const glm::dvec3& origin) :
		origin(origin) {

		const siv::PerlinNoise perlin{std::random_device{}};
		const siv::PerlinNoise perlin2{std::random_device{}};
		const siv::PerlinNoise perlin3{std::random_device{}};

		for (size_t x = 0; x < CHUNK_SIZE_X; x++)
		{
			for (size_t z = 0; z < CHUNK_SIZE_Z; z++)
			{
				int16_t genHeight = static_cast<int>(perlin.normalizedOctave2D(x * 0.01f, z * 0.01f, 1) * 20);
				genHeight += static_cast<int>(perlin2.normalizedOctave2D(x * 0.02f, z * 0.02f, 4) * 30);
				genHeight += static_cast<int>(perlin3.normalizedOctave2D(x * 0.004f, z * 0.004f, 1) * 80);
				genHeight += static_cast<int>(perlin3.normalizedOctave2D(x * 0.05f, z * 0.05f, 1) * 150);

				visibleBlocks[x][z].emplace_back(genHeight); // generate the top block

				// Since the X+ and Z+ blocks arent generated yet, we need to go backwards compared to the rendering.
				if (x > 0) {

					// index 0 will always be the top block
					// Calculates the height difference between current block and block x - 1
					int16_t heightDiff = visibleBlocks[x][z][0] - visibleBlocks[x - 1][z][0];

					if (heightDiff < 0) // if it is equal to 0, the positive branch will not run either
						for (int16_t i = -1; i > heightDiff; i--)
						{
							visibleBlocks[x][z].emplace_back(genHeight + i);
						}
					else
						for (int16_t i = 1; i < heightDiff; i++)
						{
							visibleBlocks[x][z].emplace_back(genHeight + i);
						}

				}
			}
		}
	}

	// Its a bit inneficient to do the rendering separately so maybe combine them at some point
	std::shared_ptr<ChunkRender> RenderChunk(Chunk& chunk, const OpenGL::ShaderProgram& p) {
		std::vector<ChunkVertex> vertices;

		for (size_t x = 0; x < CHUNK_SIZE_X; x++)
		{
			for (size_t z = 0; z < CHUNK_SIZE_Z; z++)
			{
				const int16_t height = chunk.visibleBlocks[x][z][0];

				vertices.emplace_back(glm::vec3(x - 0.5f, height, z - 0.5f), glm::vec3(0, 1, 0));
				vertices.emplace_back(glm::vec3(x + 0.5f, height, z - 0.5f), glm::vec3(0, 1, 0));
				vertices.emplace_back(glm::vec3(x - 0.5f, height, z + 0.5f), glm::vec3(0, 1, 0));

				vertices.emplace_back(glm::vec3(x - 0.5f, height, z + 0.5f), glm::vec3(0, 1, 0));
				vertices.emplace_back(glm::vec3(x + 0.5f, height, z - 0.5f), glm::vec3(0, 1, 0));
				vertices.emplace_back(glm::vec3(x + 0.5f, height, z + 0.5f), glm::vec3(0, 1, 0));

				// X+ face

				if (x < CHUNK_SIZE_X - 1) {

					// height diff is next - current e.g 20 - 18 = 2
					int16_t heightDiff = chunk.visibleBlocks[x + 1][z][0] - chunk.visibleBlocks[x][z][0];

					if (heightDiff < 0) // if it is equal to 0, the positive branch will not run either
						for (int16_t i = 0; i > heightDiff; i--)

						{
							glm::fvec3 colour = glm::fvec3(0.0f, 0.6f, 0.0f);
							if (abs(i) > 0)
								if (abs(i) > 2)
									colour = glm::fvec3(0.4f, 0.4f, 0.4f);
								else
									colour = glm::fvec3(0.6f, 0.3f, 0.2f);

							vertices.emplace_back(glm::vec3(x + 0.5f, height + i, z - 0.5f), colour);
							vertices.emplace_back(glm::vec3(x + 0.5f, (height - 1) + i, z - 0.5f), colour);
							vertices.emplace_back(glm::vec3(x + 0.5f, height + i, z + 0.5f), colour);

							vertices.emplace_back(glm::vec3(x + 0.5f, height + i, z + 0.5f), colour);
							vertices.emplace_back(glm::vec3(x + 0.5f, (height - 1) + i, z - 0.5f), colour);
							vertices.emplace_back(glm::vec3(x + 0.5f, (height - 1) + i, z + 0.5f), colour);
						}
					else
						for (int16_t i = 0; i < heightDiff; i++)
						{
							glm::fvec3 colour = glm::fvec3(0.0f, 0.6f, 0.0f);
							if (i > 0)
								if (i > 2)
									colour = glm::fvec3(0.4f, 0.4f, 0.4f);
								else
									colour = glm::fvec3(0.6f, 0.3f, 0.2f);
							vertices.emplace_back(glm::vec3(x + 0.5f, height + i, z - 0.5f), colour);
							vertices.emplace_back(glm::vec3(x + 0.5f, (height + 1) + i, z - 0.5f), colour);
							vertices.emplace_back(glm::vec3(x + 0.5f, height + i, z + 0.5f), colour);

							vertices.emplace_back(glm::vec3(x + 0.5f, height + i, z + 0.5f), colour);
							vertices.emplace_back(glm::vec3(x + 0.5f, (height + 1) + i, z - 0.5f), colour);
							vertices.emplace_back(glm::vec3(x + 0.5f, (height + 1) + i, z + 0.5f), colour);
						}
				}

				// The only reason to need 2 for loops each is to account for a negative for loop and a positive for loop. maybe this can be fixed?

				// Z+ face

				if (z < CHUNK_SIZE_Z - 1) {

					// height diff is next - current e.g 20 - 18 = 2
					int16_t heightDiff = chunk.visibleBlocks[x][z + 1][0] - chunk.visibleBlocks[x][z][0];

					if (heightDiff < 0) // if it is equal to 0, the positive branch will not run either
						for (int16_t i = 0; i > heightDiff; i--)
						{
							glm::fvec3 colour = glm::fvec3(0.0f, 0.6f, 0.0f);
							if (abs(i) > 0)
								if (abs(i) > 2)
									colour = glm::fvec3(0.4f, 0.4f, 0.4f);
								else
									colour = glm::fvec3(0.6f, 0.3f, 0.2f);
							vertices.emplace_back(glm::vec3(x + 0.5f, height + i, z + 0.5f), colour);
							vertices.emplace_back(glm::vec3(x - 0.5f, (height - 1) + i, z + 0.5f), colour);
							vertices.emplace_back(glm::vec3(x - 0.5f, height + i, z + 0.5f), colour);

							vertices.emplace_back(glm::vec3(x + 0.5f, (height - 1) + i, z + 0.5f), colour);
							vertices.emplace_back(glm::vec3(x - 0.5f, (height - 1) + i, z + 0.5f), colour);
							vertices.emplace_back(glm::vec3(x + 0.5f, height + i, z + 0.5f), colour);
						}
					else
						for (int16_t i = 0; i < heightDiff; i++)
						{
							glm::fvec3 colour = glm::fvec3(0.0f, 0.6f, 0.0f);
							if (i > 0)
								if (i > 2)
									colour = glm::fvec3(0.4f, 0.4f, 0.4f);
								else
									colour = glm::fvec3(0.6f, 0.3f, 0.2f);
							vertices.emplace_back(glm::vec3(x + 0.5f, height + i, z + 0.5f), colour);
							vertices.emplace_back(glm::vec3(x - 0.5f, (height + 1) + i, z + 0.5f), colour);
							vertices.emplace_back(glm::vec3(x - 0.5f, height + i, z + 0.5f), colour);

							vertices.emplace_back(glm::vec3(x + 0.5f, (height + 1) + i, z + 0.5f), colour);
							vertices.emplace_back(glm::vec3(x - 0.5f, (height + 1) + i, z + 0.5f), colour);
							vertices.emplace_back(glm::vec3(x + 0.5f, height + i, z + 0.5f), colour);
						}
				}


				// Z+ face

			}
		}

		OpenGL::Buffer buffer;
		buffer.CreateImmutableBuffer(vertices.size() * sizeof(ChunkVertex), &vertices[0]);


		std::shared_ptr<ChunkRender> render = std::make_shared<ChunkRender>();
		render->vArray.SetVertexBuffer(buffer, p.GetLayout(), 0, 0);

		render->info.count = static_cast<int32_t>(vertices.size());
		render->info.first = 0;
		render->info.mode = OpenGL::DrawMode::TRIANGLES;

		return render;

	}
}
