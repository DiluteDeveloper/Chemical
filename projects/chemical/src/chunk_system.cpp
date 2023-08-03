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
				genHeight += static_cast<int>(perlin3.normalizedOctave2D(x * 0.004f, z * 0.004f, 1) * 40);
				genHeight += static_cast<int>(perlin3.normalizedOctave2D(x * 0.009f, z * 0.009f, 1) * 50);

				for (int16_t i = genHeight; i > CHUNK_BOTTOM; i--)
				{
					if (i <= genHeight - 1) {
						if (i <= genHeight - 4)
							blocks[x][z][i] = Block(glm::fvec3(0.38f, 0.38f, 0.38f));
						else
							blocks[x][z][i] = Block(glm::fvec3(0.6f, 0.3f, 0.1f));
					}
					else
						blocks[x][z][i] = Block(glm::fvec3(0.0f, 1.0f, 0.0f));
				}
			}
		}
	}

	void Chunk::RemoveBlock(glm::ivec3 position) {
		if (position.x < CHUNK_SIZE_X - 1 && position.z < CHUNK_SIZE_Z - 1) {
			blocks[position.x][position.z].erase(position.y);
		}
	}

	void Chunk::AddBlock(glm::i16vec3 position, glm::fvec3 colour) {
		if (!blocks[position.x][position.z].contains(position.y))
			blocks[position.x][position.z][position.y] = Block(colour);
	}

	// Its a bit inneficient to do the rendering separately so maybe combine them at some point
	std::shared_ptr<ChunkRender> RenderChunk(Chunk& chunk, const OpenGL::ShaderProgram& p) {
		std::vector<ChunkVertex> vertices;

		for (size_t x = 0; x < CHUNK_SIZE_X; x++)
		{
			for (size_t z = 0; z < CHUNK_SIZE_Z; z++)
			{
				for (auto const& [y, block] : chunk.blocks[x][z]) {

					// TOP
					if (!chunk.blocks[x][z].contains(static_cast<int16_t>(y + 1))) {
						vertices.emplace_back(glm::vec3(x, y + 1, z), block.colour);
						vertices.emplace_back(glm::vec3(x + 1, y + 1, z), block.colour);
						vertices.emplace_back(glm::vec3(x, y + 1, z + 1), block.colour);

						vertices.emplace_back(glm::vec3(x, y + 1, z + 1), block.colour);
						vertices.emplace_back(glm::vec3(x + 1, y + 1, z), block.colour);
						vertices.emplace_back(glm::vec3(x + 1, y + 1, z + 1), block.colour);
					}
					// BOTTOM
					if (!chunk.blocks[x][z].contains(static_cast<int16_t>(y - 1))) {
						vertices.emplace_back(glm::vec3(x + 1, y, z), block.colour);
						vertices.emplace_back(glm::vec3(x, y, z), block.colour);
						vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);

						vertices.emplace_back(glm::vec3(x + 1, y, z), block.colour);
						vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);
						vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);
					}

					if (x < CHUNK_SIZE_X - 1) {
						if (!chunk.blocks[x + 1][z].contains(static_cast<int16_t>(y))) {

							vertices.emplace_back(glm::vec3(x + 1, y + 1, z), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);

							vertices.emplace_back(glm::vec3(x + 1, y + 1, z), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y + 1, z + 1), block.colour);
						}
					}

					if (x > 0) {
						if (!chunk.blocks[x - 1][z].contains(static_cast<int16_t>(y))) {

							vertices.emplace_back(glm::vec3(x, y, z), block.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z), block.colour);
							vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);

							vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z), block.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z + 1), block.colour);
						}
					}

					if (z < CHUNK_SIZE_Z - 1) {
						if (!chunk.blocks[x][z + 1].contains(static_cast<int16_t>(y))) {

							vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);
							
							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y + 1, z + 1), block.colour);
						}
					}

					if (z > 0) {
						if (!chunk.blocks[x][z - 1].contains(static_cast<int16_t>(y))) {

							vertices.emplace_back(glm::vec3(x, y + 1, z), block.colour);
							vertices.emplace_back(glm::vec3(x, y, z), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z), block.colour);

							vertices.emplace_back(glm::vec3(x, y + 1, z), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y + 1, z), block.colour);
						}
					}
				}

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
