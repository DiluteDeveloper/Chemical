#include "pch.h"
#include <perlin/PerlinNoise.hpp>

#include "chunk_system.h"
#include "graphics/opengl/buffer.h"


namespace Chemical {


	// std::array indexer is a uint64 when it doesnt need to be
	std::array<BlockData, 3> blockData;

	void SetBlockData() {
		// stone
		blockData[0].colour = glm::fvec3(0.38f, 0.38f, 0.38f);
		// dirt
		blockData[1].colour = glm::fvec3(0.6f, 0.3f, 0.1f);
		// grass 
		blockData[2].colour = glm::fvec3(0, 1, 0);
	}
	const BlockData& GetBlockData(BlockID blockID) {
		return blockData[blockID];
	}

	Chunk::Chunk(const glm::dvec3& origin, uint32_t seed) :
		origin(origin) {

		const siv::PerlinNoise perlin{seed};
		const siv::PerlinNoise perlin2{seed + 500}; // lazy method
		const siv::PerlinNoise perlin3{seed + 5000}; // lazy method

		for (size_t x = 0; x < CHUNK_SIZE_X; x++)
		{
			for (size_t z = 0; z < CHUNK_SIZE_Z; z++)
			{
				
				// goes from 0-20y
				int16_t genHeight = static_cast<int16_t>(perlin.octave2D_01(x * 0.02f, z * 0.02f, 4) * 20);
				// goes from -80-80y
				genHeight += static_cast<int16_t>((perlin2.octave2D_11(x * 0.005f, z * 0.005f, 4)* 120) + 60);

				// preallocation
				blocks[x][z].reserve(genHeight - CHUNK_BOTTOM);

				for (int16_t i = genHeight; i > CHUNK_BOTTOM; i--)
				{
					if (i <= genHeight - 1) {
						if (i <= genHeight - 4)
							blocks[x][z][i] = BlockID(0);
						else
							blocks[x][z][i] = BlockID(1);
					}
					else
						blocks[x][z][i] = BlockID(2);
				}
			}
		}
	}

	// Its a bit inneficient to do the rendering separately so maybe combine them at some point
	std::shared_ptr<ChunkRender> RenderChunk(const std::unique_ptr<Chunk>& chunk, const OpenGL::ShaderProgram& p) {
		std::vector<ChunkVertex> vertices;

		for (size_t x = 0; x < CHUNK_SIZE_X; x++)
		{
			for (size_t z = 0; z < CHUNK_SIZE_Z; z++)
			{
				for (auto const& [y, blockID] : chunk->blocks[x][z]) {

					const BlockData& bd = GetBlockData(blockID);
					// TOP
					if (!chunk->blocks[x][z].contains(static_cast<int16_t>(y + 1))) {
						vertices.emplace_back(glm::vec3(x, y + 1, z), bd.colour);
						vertices.emplace_back(glm::vec3(x + 1, y + 1, z), bd.colour);
						vertices.emplace_back(glm::vec3(x, y + 1, z + 1), bd.colour);

						vertices.emplace_back(glm::vec3(x, y + 1, z + 1), bd.colour);
						vertices.emplace_back(glm::vec3(x + 1, y + 1, z), bd.colour);
						vertices.emplace_back(glm::vec3(x + 1, y + 1, z + 1), bd.colour);
					}
					// BOTTOM
					if (!chunk->blocks[x][z].contains(static_cast<int16_t>(y - 1))) {
						vertices.emplace_back(glm::vec3(x + 1, y, z), bd.colour);
						vertices.emplace_back(glm::vec3(x, y, z), bd.colour);
						vertices.emplace_back(glm::vec3(x, y, z + 1), bd.colour);

						vertices.emplace_back(glm::vec3(x + 1, y, z), bd.colour);
						vertices.emplace_back(glm::vec3(x, y, z + 1), bd.colour);
						vertices.emplace_back(glm::vec3(x + 1, y, z + 1), bd.colour);
					}

					if (x < CHUNK_SIZE_X - 1) {
						if (!chunk->blocks[x + 1][z].contains(static_cast<int16_t>(y))) {

							vertices.emplace_back(glm::vec3(x + 1, y + 1, z), bd.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z), bd.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), bd.colour);

							vertices.emplace_back(glm::vec3(x + 1, y + 1, z), bd.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), bd.colour);
							vertices.emplace_back(glm::vec3(x + 1, y + 1, z + 1), bd.colour);
						}
					}

					if (x > 0) {
						if (!chunk->blocks[x - 1][z].contains(static_cast<int16_t>(y))) {

							vertices.emplace_back(glm::vec3(x, y, z), bd.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z), bd.colour);
							vertices.emplace_back(glm::vec3(x, y, z + 1), bd.colour);

							vertices.emplace_back(glm::vec3(x, y, z + 1), bd.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z), bd.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z + 1), bd.colour);
						}
					}

					if (z < CHUNK_SIZE_Z - 1) {
						if (!chunk->blocks[x][z + 1].contains(static_cast<int16_t>(y))) {

							vertices.emplace_back(glm::vec3(x, y, z + 1), bd.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z + 1), bd.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), bd.colour);
							
							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), bd.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z + 1), bd.colour);
							vertices.emplace_back(glm::vec3(x + 1, y + 1, z + 1), bd.colour);
						}
					}

					if (z > 0) {
						if (!chunk->blocks[x][z - 1].contains(static_cast<int16_t>(y))) {

							vertices.emplace_back(glm::vec3(x, y + 1, z), bd.colour);
							vertices.emplace_back(glm::vec3(x, y, z), bd.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z), bd.colour);

							vertices.emplace_back(glm::vec3(x, y + 1, z), bd.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z), bd.colour);
							vertices.emplace_back(glm::vec3(x + 1, y + 1, z), bd.colour);
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
