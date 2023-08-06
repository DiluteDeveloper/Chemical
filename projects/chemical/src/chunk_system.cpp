#include "pch.h"
#include <perlin/PerlinNoise.hpp>

#include "chunk_system.h"
#include "graphics/opengl/buffer.h"
#include "util/filestream.h"

#include "core/core.h"


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

	std::unique_ptr<Chunk> Chunk::CreateChunk(const glm::ivec2& origin, uint32_t seed) {

		std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>();
		chunk->origin = origin;

		const siv::PerlinNoise perlin{seed};
		const siv::PerlinNoise perlin2{seed + 500}; // lazy method
		const siv::PerlinNoise perlin3{seed + 5000}; // lazy method

		for (size_t x = 0; x < CHUNK_SIZE_X; x++)
		{
			for (size_t z = 0; z < CHUNK_SIZE_Z; z++)
			{
				
				// goes from 5-25y
				int16_t genHeight = static_cast<int16_t>((perlin.octave2D_01((origin.x + x) * 0.02f, (origin.y + z) * 0.02f, 4) * 20) + 5);
				genHeight += static_cast<int16_t>((perlin2.octave2D_01((origin.x + x) * 0.01f, (origin.y + z) * 0.01f, 4) * 60) + 5);

				// reverse iterator to go from terrain height - 0(bottom y of the chunk)
				// If genHeight is past CHUNK_SIZE_Y it will crash
				for (int16_t y = genHeight; y >= 0; y--)
				{
					/*if (x % 2 == 0 && z % 2 == 0)
						chunk->blocks[Dimension(x, y, z)] = BlockType::Bedrock;
					if (!(x % 2 == 0) && z % 2 == 0)
						chunk->blocks[Dimension(x, y, z)] = BlockType::Grass;
					if (x % 2 == 0 && !(z % 2 == 0))
						chunk->blocks[Dimension(x, y, z)] = BlockType::Dirt;
					if (!(x % 2 == 0) && !(z % 2 == 0))
						chunk->blocks[Dimension(x, y, z)] = BlockType::Stone;*/

					if (y == 0) {
						chunk->blocks[Dimension(x, y, z)] = BlockType::Bedrock;
						continue;
					}

					
					if (y <= genHeight - 1) {
						if (y <= genHeight - 4)
							chunk->blocks[Dimension(x,y,z)] = BlockType::Stone;
						else
							chunk->blocks[Dimension(x, y, z)] = BlockType::Dirt;
					}
					else
						chunk->blocks[Dimension(x, y, z)] = BlockType::Grass;
				}
			}
		}
		return chunk;
	}

	std::unique_ptr<ChunkLoader> ChunkLoader::CreateChunkLoader(uint8_t render_distance, uint32_t seed) {

		std::unique_ptr<ChunkLoader> chunk_loader = std::make_unique<ChunkLoader>();

		chunk_loader->render_distance = render_distance;
		chunk_loader->seed = seed;
		for (size_t i = 0; i < render_distance; i++)
		{
			chunk_loader->loaded_chunks.emplace_back(Chunk::CreateChunk(glm::ivec2(i,i), seed));
		}

		return chunk_loader;
		//if (render_distance % 2 == 0) // if rd even 
		//{

		//}
		//else { // if rd odd

	}
	std::unique_ptr<ChunkLoader> ChunkLoader::CreateChunkLoader(uint32_t seed) {
		std::unique_ptr<ChunkLoader> chunk_loader = std::make_unique<ChunkLoader>();

		chunk_loader->seed = seed;
		for (size_t i = 0; i < chunk_loader->render_distance; i++)
		{
			auto& chunk = chunk_loader->loaded_chunks.emplace_back(Chunk::CreateChunk(glm::ivec2(i, i), seed));
			chunk_loader->renderer.GenerateChunkMesh(chunk);
		}

		return chunk_loader;
	}


	void ChunkLoader::Update() {

		renderer.RenderChunks(loaded_chunks);
	}

	ChunkRenderer::ChunkRenderer() {
		OpenGL::Shader vertex_chunk_shader(Util::ReadFile("resources/shaders/chunk_shader.vert").c_str(), OpenGL::ShaderType::VERTEX_SHADER);
		OpenGL::Shader fragment_chunk_shader(Util::ReadFile("resources/shaders/chunk_shader.frag").c_str(), OpenGL::ShaderType::FRAGMENT_SHADER);

		OpenGL::VertexLayout vertex_layout;
		vertex_layout.AddAttribute(OpenGL::VertexAttribute{3, 0, OpenGL::DataType::FLOAT, OpenGL::DataTransformation::FLOAT});
		vertex_layout.AddAttribute(OpenGL::VertexAttribute{3, 3 * sizeof(float), OpenGL::DataType::FLOAT, OpenGL::DataTransformation::FLOAT});

		chunk_shader = std::make_unique<OpenGL::ShaderProgram>(std::initializer_list<
			const OpenGL::Shader*>{ &vertex_chunk_shader, & fragment_chunk_shader }, vertex_layout);

		chunk_shader->SetUniformMatrix4FV("v_proj", 1, false, &Core::projection[0][0]); 
	}

	void ChunkRenderer::RenderChunk(const std::unique_ptr<Chunk>& chunk) {

		chunk_shader->BindProgram();
		chunk_shader->SetUniformMatrix4FV("v_view", 1, false, &glm::inverse(Core::player_transform.GetTransform())[0][0]);

		chunk_shader->SetUniform2IV("v_chunk_origin", 1, &chunk->origin[0]);

		chunk->mesh.v_array.Bind();
		chunk->mesh.v_array.DrawArrays(chunk->mesh.info);
	}

	void ChunkRenderer::RenderChunks(const std::vector< std::unique_ptr<Chunk>>& chunks) {
		chunk_shader->BindProgram();
		chunk_shader->SetUniformMatrix4FV("v_view", 1, false, &glm::inverse(Core::player_transform.GetTransform())[0][0]);

		for (const auto& chunk : chunks) {
			chunk_shader->SetUniform2IV("v_chunk_origin", 1, &chunk->origin[0]);

			chunk->mesh.v_array.Bind();
			chunk->mesh.v_array.DrawArrays(chunk->mesh.info);
		}
	}

	void ChunkRenderer::GenerateChunkMesh(const std::unique_ptr<Chunk>& chunk) {
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

		chunk->mesh.v_array.SetVertexBuffer(buffer, chunk_shader->GetLayout(), 0, 0);

		chunk->mesh.info.count = static_cast<int32_t>(vertices.size());
		chunk->mesh.info.first = 0;
		chunk->mesh.info.mode = OpenGL::DrawMode::TRIANGLES;
	}
}
