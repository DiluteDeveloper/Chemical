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
	uint32_t Dimension(uint8_t x, uint16_t y, uint8_t z) {
		return ((y * CHUNK_SIZE_X * CHUNK_SIZE_Z) + (z * CHUNK_SIZE_X) + x);
	}

	std::unique_ptr<Chunk> Chunk::CreateChunk(const glm::ivec2& origin, uint32_t seed) {

		std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>();
		chunk->origin = origin;

		const siv::PerlinNoise perlin{seed};

		for (uint8_t x = 0; x < CHUNK_SIZE_X; x++)
		{
			for (uint8_t z = 0; z < CHUNK_SIZE_Z; z++)
			{
				
				// goes from 0-49y
				int16_t genHeight = static_cast<int16_t>((perlin.octave2D_01((origin.x + x) * 0.02f, (origin.y + z) * 0.02f, 4) * 71));

				// reverse iterator to go from terrain height - 0(bottom y of the chunk)
				// If genHeight is past CHUNK_SIZE_Y it will crash
				for (int16_t y = genHeight; y >= 0; y--)
				{

					//if (y == 0) {
					//	chunk->blocks[Dimension(x, y, z)] = BlockType::Bedrock;
					//	continue;
					//}

					
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


	Chunk* ChunkOrNull(const std::vector<std::unique_ptr<Chunk>>& chunks, uint32_t index) {
		if (index >= 0 && index < chunks.size())
			return chunks[index].get();
		else
			return nullptr;
	}

	std::unique_ptr<ChunkLoader> ChunkLoader::CreateChunkLoader(uint8_t render_distance, uint32_t seed) {

		std::unique_ptr<ChunkLoader> chunk_loader = std::make_unique<ChunkLoader>();

		chunk_loader->seed = seed;
		chunk_loader->render_distance = render_distance;
		for (size_t x = 0; x < chunk_loader->render_distance; x++)
		{
			for (size_t z = 0; z < chunk_loader->render_distance; z++)
			{
				auto& chunk = chunk_loader->loaded_chunks.emplace_back(Chunk::CreateChunk(glm::ivec2(x * CHUNK_SIZE_X, z * CHUNK_SIZE_Z), seed));
			}



		}
		for (size_t x = 0; x < chunk_loader->render_distance; x++)
		{
			for (size_t z = 0; z < chunk_loader->render_distance; z++)
			{
				std::array<const Chunk*, 4> edges = {
					ChunkOrNull(chunk_loader->loaded_chunks, z + (x * chunk_loader->render_distance) - chunk_loader->render_distance),
					ChunkOrNull(chunk_loader->loaded_chunks, z + (x * chunk_loader->render_distance) + 1),
					ChunkOrNull(chunk_loader->loaded_chunks, z + (x * chunk_loader->render_distance) + chunk_loader->render_distance),
					ChunkOrNull(chunk_loader->loaded_chunks, z + (x * chunk_loader->render_distance) - 1)
				};
				chunk_loader->renderer.GenerateChunkMeshEdged(chunk_loader->loaded_chunks[z + (x * chunk_loader->render_distance)], edges);
			}

		}

		return chunk_loader;

	}
	std::unique_ptr<ChunkLoader> ChunkLoader::CreateChunkLoader(uint32_t seed) {
		std::unique_ptr<ChunkLoader> chunk_loader = std::make_unique<ChunkLoader>();

		chunk_loader->seed = seed;
		for (size_t x = 0; x < chunk_loader->render_distance; x++)
		{
			for (size_t z = 0; z < chunk_loader->render_distance; z++)
			{
				auto& chunk = chunk_loader->loaded_chunks.emplace_back(Chunk::CreateChunk(glm::ivec2(x * CHUNK_SIZE_X, z * CHUNK_SIZE_Z), seed));
			}



		}
		for (size_t x = 0; x < chunk_loader->render_distance; x++)
		{
			for (size_t z = 0; z < chunk_loader->render_distance; z++)
			{
				std::array<const Chunk*, 4> edges = {
					ChunkOrNull(chunk_loader->loaded_chunks, z + (x * chunk_loader->render_distance) - chunk_loader->render_distance),
					ChunkOrNull(chunk_loader->loaded_chunks, z + (x * chunk_loader->render_distance) + 1),
					ChunkOrNull(chunk_loader->loaded_chunks, z + (x * chunk_loader->render_distance) + chunk_loader->render_distance),
					ChunkOrNull(chunk_loader->loaded_chunks, z + (x * chunk_loader->render_distance) - 1)
				};
				chunk_loader->renderer.GenerateChunkMeshEdged(chunk_loader->loaded_chunks[z + (x * chunk_loader->render_distance)], edges);
			}

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

	void ChunkRenderer::RenderChunks(const std::vector<std::unique_ptr<Chunk>>& chunks) {
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

		for (uint8_t x = 0; x < CHUNK_SIZE_X; x++)
		{
			for (uint8_t z = 0; z < CHUNK_SIZE_Z; z++)
			{
				for (uint16_t y = 0; y < CHUNK_SIZE_Y; y++)
				{
					BlockType block_type = static_cast<BlockType>(chunk->blocks[Dimension(x, y, z)]);
					if (block_type != BlockType::Air) // air
					{
						const Block& block = GetBlock(block_type);
						// TOP
						if (y < CHUNK_SIZE_Y - 1 && chunk->blocks[Dimension(x, y + 1, z)] == BlockType::Air) {
							vertices.emplace_back(glm::fvec3(x, y + 1, z), block.colour);
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

		// Creates a buffer sized to the number of vertices
		chunk->mesh.v_buffer_size = static_cast<uint32_t>(sizeof(ChunkVertex) * vertices.size());

		// not sure about dynamic draw
		chunk->mesh.v_buffer.CreateMutableBuffer(chunk->mesh.v_buffer_size, &vertices[0], OpenGL::BufferDataFlags::DYNAMIC_DRAW);

		chunk->mesh.v_array.SetVertexBuffer(chunk->mesh.v_buffer, chunk_shader->GetLayout(), 0, 0);

		chunk->mesh.info.count = static_cast<int32_t>(vertices.size());
		chunk->mesh.info.first = 0;
		chunk->mesh.info.mode = OpenGL::DrawMode::TRIANGLES;
	}

	void ChunkRenderer::GenerateChunkMeshEdged(const std::unique_ptr<Chunk>& chunk, const std::array<const Chunk*, 4> edges) {
		std::vector<ChunkVertex> vertices;

		for (uint8_t x = 0; x < CHUNK_SIZE_X; x++)
		{
			for (uint8_t z = 0; z < CHUNK_SIZE_Z; z++)
			{
				for (uint16_t y = 0; y < CHUNK_SIZE_Y; y++)
				{
					BlockType block_type = static_cast<BlockType>(chunk->blocks[Dimension(x, y, z)]);
					if (block_type != BlockType::Air) // air
					{
						const Block& block = GetBlock(block_type);
						// TOP
						if (y < CHUNK_SIZE_Y - 1 && chunk->blocks[Dimension(x, y + 1, z)] == BlockType::Air) {
							vertices.emplace_back(glm::fvec3(x, y + 1, z), block.colour);
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

						if (x < CHUNK_SIZE_X - 1) {
							if (chunk->blocks[Dimension(x + 1, y, z)] == BlockType::Air) {

								vertices.emplace_back(glm::vec3(x + 1, y + 1, z), block.colour);
								vertices.emplace_back(glm::vec3(x + 1, y, z), block.colour);
								vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);

								vertices.emplace_back(glm::vec3(x + 1, y + 1, z), block.colour);
								vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);
								vertices.emplace_back(glm::vec3(x + 1, y + 1, z + 1), block.colour);
							}
						}
						else {
							if (edges[2] != nullptr) {
								if (edges[2]->blocks[Dimension(0, y, z)] == BlockType::Air) {
									vertices.emplace_back(glm::vec3(x + 1, y + 1, z), block.colour);
									vertices.emplace_back(glm::vec3(x + 1, y, z), block.colour);
									vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);

									vertices.emplace_back(glm::vec3(x + 1, y + 1, z), block.colour);
									vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);
									vertices.emplace_back(glm::vec3(x + 1, y + 1, z + 1), block.colour);
								}
							}
						}

						if (x > 0) {
							if (chunk->blocks[Dimension(x - 1, y, z)] == BlockType::Air) {

								vertices.emplace_back(glm::vec3(x, y, z), block.colour);
								vertices.emplace_back(glm::vec3(x, y + 1, z), block.colour);
								vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);

								vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);
								vertices.emplace_back(glm::vec3(x, y + 1, z), block.colour);
								vertices.emplace_back(glm::vec3(x, y + 1, z + 1), block.colour);
							}
						}
						else {
							if (edges[0] != nullptr) {
								if (edges[0]->blocks[Dimension(CHUNK_SIZE_X - 1, y, z)] == BlockType::Air) {
 									vertices.emplace_back(glm::vec3(x, y, z), block.colour);
									vertices.emplace_back(glm::vec3(x, y + 1, z), block.colour);
									vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);

									vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);
									vertices.emplace_back(glm::vec3(x, y + 1, z), block.colour);
									vertices.emplace_back(glm::vec3(x, y + 1, z + 1), block.colour);
								}
							}
						}
						if (z < CHUNK_SIZE_Z - 1) {
							if (chunk->blocks[Dimension(x, y, z + 1)] == BlockType::Air) {

								vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);
								vertices.emplace_back(glm::vec3(x, y + 1, z + 1), block.colour);
								vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);

								vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);
								vertices.emplace_back(glm::vec3(x, y + 1, z + 1), block.colour);
								vertices.emplace_back(glm::vec3(x + 1, y + 1, z + 1), block.colour);
							}
						}
						else {
							if (edges[1] != nullptr) {
								if (edges[1]->blocks[Dimension(x, y, 0)] == BlockType::Air) {

									vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);
									vertices.emplace_back(glm::vec3(x, y + 1, z + 1), block.colour);
									vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);

									vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);
									vertices.emplace_back(glm::vec3(x, y + 1, z + 1), block.colour);
									vertices.emplace_back(glm::vec3(x + 1, y + 1, z + 1), block.colour);
								}
							}
						}

						if (z > 0) {
							if (chunk->blocks[Dimension(x, y, z - 1)] == BlockType::Air) {

								vertices.emplace_back(glm::vec3(x, y + 1, z), block.colour);
								vertices.emplace_back(glm::vec3(x, y, z), block.colour);
								vertices.emplace_back(glm::vec3(x + 1, y, z), block.colour);

								vertices.emplace_back(glm::vec3(x, y + 1, z), block.colour);
								vertices.emplace_back(glm::vec3(x + 1, y, z), block.colour);
								vertices.emplace_back(glm::vec3(x + 1, y + 1, z), block.colour);
							}
						}
						else {
							if (edges[3] != nullptr) {
								if (edges[3]->blocks[Dimension(x, y, CHUNK_SIZE_Z - 1)] == BlockType::Air) {

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

			}
		}

		// Creates a buffer sized to the number of vertices
		chunk->mesh.v_buffer_size = static_cast<uint32_t>((sizeof(ChunkVertex) * vertices.size()));

		// not sure about dynamic draw
		chunk->mesh.v_buffer.CreateMutableBuffer(chunk->mesh.v_buffer_size, &vertices[0], OpenGL::BufferDataFlags::DYNAMIC_DRAW);

		chunk->mesh.v_array.SetVertexBuffer(chunk->mesh.v_buffer, chunk_shader->GetLayout(), 0, 0);

		chunk->mesh.info.count = static_cast<int32_t>(vertices.size());
		chunk->mesh.info.first = 0;
		chunk->mesh.info.mode = OpenGL::DrawMode::TRIANGLES;
	}

	void ChunkRenderer::RegenerateChunkMesh(const std::unique_ptr<Chunk>& chunk) {
		std::vector<ChunkVertex> vertices;

		for (uint8_t x = 0; x < CHUNK_SIZE_X; x++)
		{
			for (uint8_t z = 0; z < CHUNK_SIZE_Z; z++)
			{
				for (uint16_t y = 0; y < CHUNK_SIZE_Y; y++)
				{
					BlockType block_type = static_cast<BlockType>(chunk->blocks[Dimension(x, y, z)]);
					if (block_type != BlockType::Air) // air
					{
						const Block& block = GetBlock(block_type);
						// TOP
						if (y < CHUNK_SIZE_Y - 1 && chunk->blocks[Dimension(x, y + 1, z)] == BlockType::Air) {
							vertices.emplace_back(glm::fvec3(x, y + 1, z), block.colour);
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

		// buffer is big enough
		if (chunk->mesh.v_buffer_size >= vertices.size()) {
			chunk->mesh.v_buffer.SetBufferData(chunk->mesh.v_buffer_size, &vertices[0], 0);
		}
		else { // buffer is not big enough
			chunk->mesh.v_buffer_size = static_cast<uint32_t>(vertices.size() * 1.2);

			chunk->mesh.v_buffer.CreateMutableBuffer(chunk->mesh.v_buffer_size, &vertices[0], OpenGL::BufferDataFlags::DYNAMIC_DRAW);
		}

		chunk->mesh.info.count = static_cast<int32_t>(vertices.size());
	}
}
