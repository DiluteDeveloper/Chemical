#include "pch.h"
#include <perlin/PerlinNoise.hpp>

#include "chunk_system.h"
#include "graphics/opengl/buffer.h"


namespace Chemical {

	/*
	
	BlockID's are stored in the thousands/millions.
	They represent an index into block_registry.
	block_registry contains all of the actual data 
	that a block needs for function.
	BlockType is basically just a macro to make
	working with different blocks easier.

	*/

	struct Block {

		glm::fvec3 colour = glm::fvec3(0.0f);
	};

	// Easier way to code BlockID's, both align with an index into blockData
	enum BlockType {
		Air = 0,
		Stone = 1,
		Dirt = 2,
		Grass = 3,
		Bedrock = 4
	};

	// std::array indexer is a uint64 when it doesnt need to be
	std::array<Block, 5> block_registry;

	void InitializeBlockData() {
		// air
		//blockData[0].colour = glm::fvec3(0.0f);
		// stone
		block_registry[BlockType::Stone].colour = glm::fvec3(0.38f, 0.38f, 0.38f);
		// dirt
		block_registry[BlockType::Dirt].colour = glm::fvec3(0.6f, 0.3f, 0.1f);
		// grass 
		block_registry[BlockType::Grass].colour = glm::fvec3(0, 1, 0);
		// bedrock
		block_registry[BlockType::Bedrock].colour = glm::fvec3(0.2, 0.2, 0.2);
	}
	const Block& GetBlock(BlockType block_type) {
		return block_registry[block_type];
	}

	// converts 3D chunk coordinates to 1D, can crash
	uint64_t Dimension(size_t x, size_t y, size_t z) {
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
						blocks[Dimension(x, y, z)] = 4;
						continue;
					}

					
					if (y <= genHeight - 1) {
						if (y <= genHeight - 4)
								blocks[Dimension(x,y,z)] = 1;
						else
							blocks[Dimension(x, y, z)] = 2;
					}
					else
						blocks[Dimension(x, y, z)] = 3;
				}
			}
		}
	}

	std::shared_ptr<ChunkMesh> RenderChunk(const std::unique_ptr<Chunk>& chunk, const OpenGL::ShaderProgram& p) {
		std::vector<ChunkVertex> vertices;

		for (size_t x = 0; x < CHUNK_SIZE_X; x++)
		{
			for (size_t z = 0; z < CHUNK_SIZE_Z; z++)
			{
				for (size_t y = 0; y < CHUNK_SIZE_Y; y++)
				{
					BlockType block_type = static_cast<BlockType>(chunk->blocks[Dimension(x, y, z)]);
					if (block_type != BlockType::Air) // air
					{
						const Block& block = GetBlock(block_type);
						// TOP
						if (y < CHUNK_SIZE_Y - 1 && chunk->blocks[Dimension(x, y + 1, z)] == BlockType::Air) {
							vertices.emplace_back(glm::vec3(x, y + 1, z), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y + 1, z), block.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z + 1), block.colour);

							vertices.emplace_back(glm::vec3(x, y + 1, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y + 1, z), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y + 1, z + 1), block.colour);
						}

						// Very special code, allows for chunk edge at the bottom.
						// If the 2nd section of the if statement is executed at bottom y of the chunk, it will crash
						// the program, so if the y is the bottom y of the chunk, the || makes it so the 2nd section
						// of the if statement will not execute and crash the program
						// big brain code or bad design? who knows
						if (y == 0 || chunk->blocks[Dimension(x, y - 1, z)] == BlockType::Air) {
							vertices.emplace_back(glm::vec3(x + 1, y, z), block.colour);
							vertices.emplace_back(glm::vec3(x, y, z), block.colour);
							vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);

							vertices.emplace_back(glm::vec3(x + 1, y, z), block.colour);
							vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);
						}

						if (x < CHUNK_SIZE_X - 1 && chunk->blocks[Dimension(x + 1, y, z)] == BlockType::Air) {

							vertices.emplace_back(glm::vec3(x + 1, y + 1, z), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);

							vertices.emplace_back(glm::vec3(x + 1, y + 1, z), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y + 1, z + 1), block.colour);
						}

						if (x > 0 && chunk->blocks[Dimension(x - 1, y, z)] == BlockType::Air) {

							vertices.emplace_back(glm::vec3(x, y, z), block.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z), block.colour);
							vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);

							vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z), block.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z + 1), block.colour);
						}

						if (z < CHUNK_SIZE_Z - 1 && chunk->blocks[Dimension(x, y, z + 1)] == BlockType::Air) {

							vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);

							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y + 1, z + 1), block.colour);
						}

						if (z > 0 && chunk->blocks[Dimension(x, y, z - 1)] == BlockType::Air) {

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


		std::shared_ptr<ChunkMesh> render = std::make_shared<ChunkMesh>();
		render->v_array.SetVertexBuffer(buffer, p.GetLayout(), 0, 0);

		render->info.count = static_cast<int32_t>(vertices.size());
		render->info.first = 0;
		render->info.mode = OpenGL::DrawMode::TRIANGLES;

		return render;

	}
}
