#include "pch.h"
#include <perlin/PerlinNoise.hpp>

#include "chunk_system.h"
#include "graphics/opengl/buffer.h"


namespace Chemical {


	// std::array indexer is a uint64 when it doesnt need to be
	std::array<BlockData, 5> blockData;

	void SetBlockData() {
		// air
		//blockData[0].colour = glm::fvec3(0.0f);
		// stone
		blockData[1].colour = glm::fvec3(0.38f, 0.38f, 0.38f);
		// dirt
		blockData[2].colour = glm::fvec3(0.6f, 0.3f, 0.1f);
		// grass 
		blockData[3].colour = glm::fvec3(0, 1, 0);
		// bedrock
		blockData[4].colour = glm::fvec3(0.2, 0.2, 0.2);
	}
	const BlockData& GetBlockData(BlockID blockID) {
		return blockData[blockID];
	}

	// converts 3D chunk coordinates to 1D, can crash
	uint64_t dimension(int x, int y, int z) {
		return ((y * CHUNK_SIZE_X * CHUNK_SIZE_Z) + (z * CHUNK_SIZE_X) + x);
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
				
				// goes from 5-25y
				int16_t genHeight = static_cast<int16_t>((perlin.octave2D_01(x * 0.08f, z * 0.08f, 4) * 20) + 5);

				// reverse iterator to go from terrain height - 0(bottom y of the chunk)
				// If genHeight is past CHUNK_SIZE_Y it will crash
				for (int16_t y = genHeight; y >= 0; y--)
				{

					if (y == 0) {
						blocks[dimension(x, y, z)] = 4;
						continue;
					}

					
					if (y <= genHeight - 1) {
						if (y <= genHeight - 4)
								blocks[dimension(x,y,z)] = 1;
						else
							blocks[dimension(x, y, z)] = 2;
					}
					else
						blocks[dimension(x, y, z)] = 3;
				}
			}
		}
	}

	std::shared_ptr<ChunkRender> RenderChunk(const std::unique_ptr<Chunk>& chunk, const OpenGL::ShaderProgram& p) {
		std::vector<ChunkVertex> vertices;

		for (size_t x = 0; x < CHUNK_SIZE_X; x++)
		{
			for (size_t z = 0; z < CHUNK_SIZE_Z; z++)
			{
				for (size_t y = 0; y < CHUNK_SIZE_Y; y++)
				{
					uint16_t blockID = chunk->blocks[dimension(x, y, z)];
					if (blockID != 0) // air
					{
						const BlockData& bd = GetBlockData(blockID);
						// TOP
						if (y < CHUNK_SIZE_Y - 1 && chunk->blocks[dimension(x, y + 1, z)] == 0) {
							vertices.emplace_back(glm::vec3(x, y + 1, z), bd.colour);
							vertices.emplace_back(glm::vec3(x + 1, y + 1, z), bd.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z + 1), bd.colour);

							vertices.emplace_back(glm::vec3(x, y + 1, z + 1), bd.colour);
							vertices.emplace_back(glm::vec3(x + 1, y + 1, z), bd.colour);
							vertices.emplace_back(glm::vec3(x + 1, y + 1, z + 1), bd.colour);
						}

						// Very special code, allows for chunk edge at the bottom.
						// If the 2nd section of the if statement is executed at bottom y of the chunk, it will crash
						// the program, so if the y is the bottom y of the chunk, the || makes it so the 2nd section
						// of the if statement will not execute and crash the program
						// big brain code or bad design? who knows
						if (y == 0 || chunk->blocks[dimension(x, y - 1, z)] == 0) {
							vertices.emplace_back(glm::vec3(x + 1, y, z), bd.colour);
							vertices.emplace_back(glm::vec3(x, y, z), bd.colour);
							vertices.emplace_back(glm::vec3(x, y, z + 1), bd.colour);

							vertices.emplace_back(glm::vec3(x + 1, y, z), bd.colour);
							vertices.emplace_back(glm::vec3(x, y, z + 1), bd.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), bd.colour);
						}

						if (x < CHUNK_SIZE_X - 1 && chunk->blocks[dimension(x + 1, y, z)] == 0) {

							vertices.emplace_back(glm::vec3(x + 1, y + 1, z), bd.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z), bd.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), bd.colour);

							vertices.emplace_back(glm::vec3(x + 1, y + 1, z), bd.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), bd.colour);
							vertices.emplace_back(glm::vec3(x + 1, y + 1, z + 1), bd.colour);
						}

						if (x > 0 && chunk->blocks[dimension(x - 1, y, z)] == 0) {

							vertices.emplace_back(glm::vec3(x, y, z), bd.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z), bd.colour);
							vertices.emplace_back(glm::vec3(x, y, z + 1), bd.colour);

							vertices.emplace_back(glm::vec3(x, y, z + 1), bd.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z), bd.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z + 1), bd.colour);
						}

						if (z < CHUNK_SIZE_Z - 1 && chunk->blocks[dimension(x, y, z + 1)] == 0) {

							vertices.emplace_back(glm::vec3(x, y, z + 1), bd.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z + 1), bd.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), bd.colour);

							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), bd.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z + 1), bd.colour);
							vertices.emplace_back(glm::vec3(x + 1, y + 1, z + 1), bd.colour);
						}

						if (z > 0 && chunk->blocks[dimension(x, y, z - 1)] == 0) {

							vertices.emplace_back(glm::vec3(x, y + 1, z), bd.colour);
							vertices.emplace_back(glm::vec3(x, y, z), bd.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z), bd.colour);

							vertices.emplace_back(glm::vec3(x, y + 1, z), bd.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z), bd.colour);
							vertices.emplace_back(glm::vec3(x + 1, y + 1, z), bd.colour);
						}
					}
				}
				/*for (auto const& [y, blockID] : chunk->blocks[x][z]) {

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
				}*/

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
