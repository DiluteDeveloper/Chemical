#include "pch.h"
#include <perlin/PerlinNoise.hpp>

#include "chunk_system.h"
#include "graphics/opengl/buffer.h"
#include "util/filestream.h"

#include "core/core.h"


namespace Chemical {

	struct Block {

		glm::fvec3 colour = glm::fvec3(0.0f);
	};

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

	// converts 3D block coordinates to 1D
	uint32_t Block3Dto1D(uint8_t x, uint16_t y, uint8_t z) {
		return ((y * CHUNK_SIZE_X * CHUNK_SIZE_Z) + (z * CHUNK_SIZE_X) + x);
	}

	Chunk::Chunk(const glm::ivec2& origin, uint32_t seed) :
	origin(origin) {
		const siv::PerlinNoise perlin{seed};

		for (uint8_t x = 0; x < CHUNK_SIZE_X; x++)
		{
			for (uint8_t z = 0; z < CHUNK_SIZE_Z; z++)
			{

				// goes from 0-71y
				int16_t genHeight = static_cast<int16_t>((perlin.octave2D_01((origin.x + x) * 0.02f, (origin.y + z) * 0.02f, 4) * 71));

				// reverse iterator to go from terrain height - 0(bottom y of the chunk)
				// If genHeight is past CHUNK_SIZE_Y it will crash
				for (int16_t y = genHeight; y >= 0; y--)
				{

					if (y == 0) {
						blocks[Block3Dto1D(x, y, z)] = BlockType::Bedrock;
						continue;
					}


					if (y <= genHeight - 1) {
						if (y <= genHeight - 4)
							blocks[Block3Dto1D(x, y, z)] = BlockType::Stone;
						else
							blocks[Block3Dto1D(x, y, z)] = BlockType::Dirt;
					}
					else
						blocks[Block3Dto1D(x, y, z)] = BlockType::Grass;
				}
			}
		}
	}

	ChunkLoader::ChunkLoader(uint32_t seed, uint8_t render_distance) :
	seed(seed), real_rd((render_distance % 2 == 0) ? render_distance : render_distance - 1) {

		glm::ivec2 player_chunk_coordinates;
		player_chunk_coordinates.x = std::floor(Core::player_transform.position.x / 16);
		player_chunk_coordinates.y = std::floor(Core::player_transform.position.z / 16);

		for (int32_t x = -real_rd; x < real_rd + 1; x++)
		{
			for (int32_t z = -real_rd; z < real_rd + 1; z++)
			{
				if (x == 0 && z == 0)
					center_chunk_origin = glm::ivec2(x + player_chunk_coordinates.x, z + player_chunk_coordinates.y);
				loaded_chunks
					[x + player_chunk_coordinates.x]
					[z + player_chunk_coordinates.y] =
						std::make_unique<Chunk>(glm::ivec2((x + player_chunk_coordinates.x) * CHUNK_SIZE_X, (z + player_chunk_coordinates.y) * CHUNK_SIZE_Z), seed);
			}

		}

		for (int32_t x = -real_rd; x < real_rd + 1; x++)
		{
			for (int32_t z = -real_rd; z < real_rd + 1; z++)
			{

				std::array<const Chunk*, 4> edges = {};

				edges[0] = (x > -real_rd) ? loaded_chunks[(x + player_chunk_coordinates.x) - 1][(z + player_chunk_coordinates.y)].get() : nullptr;
				edges[1] = (z < real_rd - 1) ? loaded_chunks[x + player_chunk_coordinates.x][(z + player_chunk_coordinates.y) + 1].get() : nullptr;
				edges[2] = (x < real_rd - 1) ? loaded_chunks[(x + player_chunk_coordinates.x) + 1][(z + player_chunk_coordinates.y)].get() : nullptr;
				edges[3] = (z > -real_rd) ? loaded_chunks[x + player_chunk_coordinates.x][(z + player_chunk_coordinates.y) - 1].get() : nullptr;

				renderer.GenerateChunkMeshEdged(loaded_chunks[x + player_chunk_coordinates.x][z + player_chunk_coordinates.y], edges);
			}
		}
	}


	void ChunkLoader::Update() {

		//LOGGER_CONSOLE_CUSTOM_MESSAGE("{}, {}", Core::player_transform.position.x, Core::player_transform.position.z);
		glm::ivec2 player_chunk_coordinates;
		player_chunk_coordinates.x = std::floor(Core::player_transform.position.x / 16);
		player_chunk_coordinates.y = std::floor(Core::player_transform.position.z / 16);

		if (player_chunk_coordinates != center_chunk_origin) {
			glm::ivec2 offset = player_chunk_coordinates - center_chunk_origin;


			if (offset.y != 0) { // the player changed z chunk
				for (int32_t i = -real_rd; i < real_rd + 1; i++)
				{

					LOGGER_CONSOLE_CUSTOM_MESSAGE("{}", i);
					loaded_chunks
						[i + player_chunk_coordinates.x]
					[player_chunk_coordinates.y + (real_rd * offset.y)] =
						std::make_unique<Chunk>(glm::ivec2((i + player_chunk_coordinates.x) * CHUNK_SIZE_X, ((player_chunk_coordinates.y + (real_rd * offset.y))) * CHUNK_SIZE_Z), seed);

					loaded_chunks[i + player_chunk_coordinates.x].erase(player_chunk_coordinates.y + ((real_rd + 1) * -offset.y));

					std::array<const Chunk*, 4> edges = {};

					//edges[0] = (i > -real_rd) ? loaded_chunks[(i + player_chunk_coordinates.x) - 1]
						//[player_chunk_coordinates.y + (real_rd * offset.y)].get() : nullptr;
					//edges[1] = (z < real_rd - 1) ? loaded_chunks[i + player_chunk_coordinates.x][((abs(player_chunk_coordinates.y) + real_rd + 1) * offset.y) + 1].get() : nullptr;
					//edges[1] = (z < real_rd - 1) ? loaded_chunks[i + player_chunk_coordinates.x][((abs(player_chunk_coordinates.y) + real_rd + 1) * offset.y) + 1].get() : nullptr;
					//edges[2] = (x < real_rd - 1) ? loaded_chunks[(i + player_chunk_coordinates.x) + 1][((abs(player_chunk_coordinates.y) + real_rd + 1) * offset.y)].get() : nullptr;
					//edges[3] = (z > -real_rd) ? loaded_chunks[i + player_chunk_coordinates.x][((abs(player_chunk_coordinates.y) + real_rd + 1) * offset.y) - 1].get() : nullptr;

					renderer.GenerateChunkMesh(loaded_chunks[i + player_chunk_coordinates.x]
						[(player_chunk_coordinates.y + (real_rd * offset.y))]);
				}
			}
			if(offset.x != 0) { // the player changed x chunk
				for (int32_t i = -real_rd; i < real_rd + 1; i++)
				{
					LOGGER_CONSOLE_CUSTOM_MESSAGE("{}", i);
					loaded_chunks
						[player_chunk_coordinates.x + (real_rd * offset.x)]
					[i + player_chunk_coordinates.y] =
						std::make_unique<Chunk>(glm::ivec2((player_chunk_coordinates.x + (real_rd * offset.x)) * CHUNK_SIZE_X, (i + player_chunk_coordinates.y) * CHUNK_SIZE_Z), seed);

					loaded_chunks[player_chunk_coordinates.x + ((real_rd + 1) * -offset.x)].erase(i + player_chunk_coordinates.y);

					std::array<const Chunk*, 4> edges = {};

					//edges[0] = (i > -real_rd) ? loaded_chunks[(i + player_chunk_coordinates.x) - 1][((abs(player_chunk_coordinates.y) + real_rd + 1) * offset.y)].get() : nullptr;
					//edges[1] = (z < real_rd - 1) ? loaded_chunks[i + player_chunk_coordinates.x][((abs(player_chunk_coordinates.y) + real_rd + 1) * offset.y) + 1].get() : nullptr;
					//edges[2] = (x < real_rd - 1) ? loaded_chunks[(i + player_chunk_coordinates.x) + 1][((abs(player_chunk_coordinates.y) + real_rd + 1) * offset.y)].get() : nullptr;
					//edges[3] = (z > -real_rd) ? loaded_chunks[i + player_chunk_coordinates.x][((abs(player_chunk_coordinates.y) + real_rd + 1) * offset.y) - 1].get() : nullptr;

					renderer.GenerateChunkMesh(loaded_chunks[player_chunk_coordinates.x + (real_rd * offset.x)]
						[i + player_chunk_coordinates.y]);
				}
			}
			center_chunk_origin = player_chunk_coordinates;
		}

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

	void ChunkRenderer::RenderChunks(const std::unordered_map<int32_t, std::unordered_map<int32_t, std::unique_ptr<Chunk>>>& chunks) {
		chunk_shader->BindProgram();
		chunk_shader->SetUniformMatrix4FV("v_view", 1, false, &glm::inverse(Core::player_transform.GetTransform())[0][0]);

		for (const auto&[x, list] : chunks) {
			for (const auto& [z, chunk] : list) {
					
				chunk_shader->SetUniform2IV("v_chunk_origin", 1, &chunk->origin[0]);

				chunk->mesh.v_array.Bind();
				chunk->mesh.v_array.DrawArrays(chunk->mesh.info);
			}

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
					BlockType block_type = static_cast<BlockType>(chunk->blocks[Block3Dto1D(x, y, z)]);
					if (block_type != BlockType::Air) // air
					{
						const Block& block = GetBlock(block_type);
						// TOP
						if (y < CHUNK_SIZE_Y - 1 && chunk->blocks[Block3Dto1D(x, y + 1, z)] == BlockType::Air) {
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
						if (y == 0 || chunk->blocks[Block3Dto1D(x, y - 1, z)] == BlockType::Air) {
							vertices.emplace_back(glm::vec3(x + 1, y, z), block.colour);
							vertices.emplace_back(glm::vec3(x, y, z), block.colour);
							vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);

							vertices.emplace_back(glm::vec3(x + 1, y, z), block.colour);
							vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);
						}

						if (x < CHUNK_SIZE_X - 1 && chunk->blocks[Block3Dto1D(x + 1, y, z)] == BlockType::Air) {

							vertices.emplace_back(glm::vec3(x + 1, y + 1, z), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);

							vertices.emplace_back(glm::vec3(x + 1, y + 1, z), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y + 1, z + 1), block.colour);
						}

						if (x > 0 && chunk->blocks[Block3Dto1D(x - 1, y, z)] == BlockType::Air) {

							vertices.emplace_back(glm::vec3(x, y, z), block.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z), block.colour);
							vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);

							vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z), block.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z + 1), block.colour);
						}

						if (z < CHUNK_SIZE_Z - 1 && chunk->blocks[Block3Dto1D(x, y, z + 1)] == BlockType::Air) {

							vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);

							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y + 1, z + 1), block.colour);
						}

						if (z > 0 && chunk->blocks[Block3Dto1D(x, y, z - 1)] == BlockType::Air) {

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
					BlockType block_type = static_cast<BlockType>(chunk->blocks[Block3Dto1D(x, y, z)]);
					if (block_type != BlockType::Air)
					{
						const Block& block = GetBlock(block_type);
						// TOP
						if (y < CHUNK_SIZE_Y - 1 && chunk->blocks[Block3Dto1D(x, y + 1, z)] == BlockType::Air) {
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
						if (y == 0 || chunk->blocks[Block3Dto1D(x, y - 1, z)] == BlockType::Air) {
							vertices.emplace_back(glm::vec3(x + 1, y, z), block.colour);
							vertices.emplace_back(glm::vec3(x, y, z), block.colour);
							vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);

							vertices.emplace_back(glm::vec3(x + 1, y, z), block.colour);
							vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);
						}

						if (x < CHUNK_SIZE_X - 1) {
							if (chunk->blocks[Block3Dto1D(x + 1, y, z)] == BlockType::Air) {

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
								if (edges[2]->blocks[Block3Dto1D(0, y, z)] == BlockType::Air) {
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
							if (chunk->blocks[Block3Dto1D(x - 1, y, z)] == BlockType::Air) {

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
								if (edges[0]->blocks[Block3Dto1D(CHUNK_SIZE_X - 1, y, z)] == BlockType::Air) {
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
							if (chunk->blocks[Block3Dto1D(x, y, z + 1)] == BlockType::Air) {

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
								if (edges[1]->blocks[Block3Dto1D(x, y, 0)] == BlockType::Air) {

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
							if (chunk->blocks[Block3Dto1D(x, y, z - 1)] == BlockType::Air) {

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
								if (edges[3]->blocks[Block3Dto1D(x, y, CHUNK_SIZE_Z - 1)] == BlockType::Air) {

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
					BlockType block_type = static_cast<BlockType>(chunk->blocks[Block3Dto1D(x, y, z)]);
					if (block_type != BlockType::Air) // air
					{
						const Block& block = GetBlock(block_type);
						// TOP
						if (y < CHUNK_SIZE_Y - 1 && chunk->blocks[Block3Dto1D(x, y + 1, z)] == BlockType::Air) {
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
						if (y == 0 || chunk->blocks[Block3Dto1D(x, y - 1, z)] == BlockType::Air) {
							vertices.emplace_back(glm::vec3(x + 1, y, z), block.colour);
							vertices.emplace_back(glm::vec3(x, y, z), block.colour);
							vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);

							vertices.emplace_back(glm::vec3(x + 1, y, z), block.colour);
							vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);
						}

						if (x < CHUNK_SIZE_X - 1 && chunk->blocks[Block3Dto1D(x + 1, y, z)] == BlockType::Air) {

							vertices.emplace_back(glm::vec3(x + 1, y + 1, z), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);

							vertices.emplace_back(glm::vec3(x + 1, y + 1, z), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y + 1, z + 1), block.colour);
						}

						if (x > 0 && chunk->blocks[Block3Dto1D(x - 1, y, z)] == BlockType::Air) {

							vertices.emplace_back(glm::vec3(x, y, z), block.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z), block.colour);
							vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);

							vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z), block.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z + 1), block.colour);
						}

						if (z < CHUNK_SIZE_Z - 1 && chunk->blocks[Block3Dto1D(x, y, z + 1)] == BlockType::Air) {

							vertices.emplace_back(glm::vec3(x, y, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);

							vertices.emplace_back(glm::vec3(x + 1, y, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x, y + 1, z + 1), block.colour);
							vertices.emplace_back(glm::vec3(x + 1, y + 1, z + 1), block.colour);
						}

						if (z > 0 && chunk->blocks[Block3Dto1D(x, y, z - 1)] == BlockType::Air) {

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
