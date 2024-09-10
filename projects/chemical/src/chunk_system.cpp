#include "pch.h"

#include <perlin/PerlinNoise.hpp>
#include <future>

#include "chunk_system.h"
#include "util/filestream.h"

#include "core/core.h"


namespace Chemical {

	Chunk::Chunk(const glm::ivec2& origin, uint32_t seed) {
		const siv::PerlinNoise perlin{seed};
		const siv::PerlinNoise perlin2{seed + 50000};

		for (uint8_t x = 0; x < CHUNK_SIZE; x++)
		{
			for (uint8_t z = 0; z < CHUNK_SIZE; z++)
			{

				int16_t genHeight = static_cast<int16_t>(
					(((perlin.octave2D_01(((origin.x * CHUNK_SIZE) + x) * 0.005f, ((origin.y * CHUNK_SIZE) + z) * 0.005f, 1) +
						perlin2.octave2D_01(((origin.x * CHUNK_SIZE) + x) * 0.005f, ((origin.y * CHUNK_SIZE) + z) * 0.005f, 6))
						/ 2)* 398) + 1);

				// reverse iterator to go from terrain height - 0(bottom y of the chunk)
				// If genHeight is past CHUNK_HEIGHT it will be empty past that height
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
		seed(seed), render_distance(render_distance) {

		// + 1 accounts for the center chunk
		for (int8_t x = -render_distance; x < render_distance + 1; x++)
		{
			for (int8_t z = -render_distance; z < render_distance + 1; z++)
			{
				loaded_chunks[x][z] = std::make_unique<Chunk>(glm::ivec2{x, z}, seed);
			}

		}
		for (int8_t x = -render_distance; x < render_distance + 1; x++)
		{
			for (int8_t z = -render_distance; z < render_distance + 1; z++)
			{
				// alloc is an allocation multiplier based on whether its possible
				// for an edge to be allocated on any given side.
				float alloc = 1.0f;
				if (z < render_distance) {
					loaded_chunks[x][z]->edges.zp_chunk = loaded_chunks[x][z + 1].get();
				}
				else alloc += 0.0625f;
				if (z > -render_distance) {
					loaded_chunks[x][z]->edges.zm_chunk = loaded_chunks[x][z - 1].get();
				}
				else alloc += 0.0625f;

				if (x < render_distance) {
					loaded_chunks[x][z]->edges.xp_chunk = loaded_chunks[x + 1][z].get();
				}
				else alloc += 0.0625f;
				if (x > -render_distance) {
					loaded_chunks[x][z]->edges.xm_chunk = loaded_chunks[x - 1][z].get();
				}
				else alloc += 0.0625f;

				renderer.SetupChunkMesh(loaded_chunks[x][z]);
				renderer.BuildChunkMesh(loaded_chunks[x][z], alloc);
			}
		}
	}

	void ChunkLoader::Update() {


		if (doChunkLoading) {
			// converts player position to chunk coordinates
			glm::ivec2 player_chunk_coordinates;
			player_chunk_coordinates.x = static_cast<int>(std::floor(Core::player_transform.position.x / CHUNK_SIZE));
			player_chunk_coordinates.y = static_cast<int>(std::floor(Core::player_transform.position.z / CHUNK_SIZE));


			if (player_chunk_coordinates != center_origin) {
				// player has moved into the bounding region of another chunk

				// get which direction the player went in
				glm::ivec2 offset = player_chunk_coordinates - center_origin;

				if (offset.y != 0) {
					// the player changed Z chunk

					int32_t z_origin = center_origin.y + ((render_distance + 1) * offset.y);

					for (int16_t x = -render_distance; x < render_distance + 1; x++)
					{
						int32_t x_origin = center_origin.x + x;
						int32_t neg_z_origin = center_origin.y + (render_distance * -offset.y);

						loaded_chunks[x_origin].erase(neg_z_origin);
						loaded_chunks[x_origin][z_origin] = std::make_unique<Chunk>(glm::ivec2{x_origin, z_origin}, seed);


						if (offset.y > 0) {
							// the player went Z+ chunk

							loaded_chunks[x_origin][z_origin]->edges.zm_chunk = loaded_chunks[x_origin][z_origin - 1].get();

							loaded_chunks[x_origin][z_origin - 1]->edges.zp_chunk = loaded_chunks[x_origin][z_origin].get();


							//std::cout << "CHUNK UPDATE X: " << x_origin << " CHUNK Z: " << z_origin - 1 << std::endl;
							renderer.UpdateChunkMeshEdges(loaded_chunks[x_origin][z_origin - 1], 1.05f);
						}

						else {
							// the player went Z- chunk

							loaded_chunks[x_origin][z_origin]->edges.zp_chunk = loaded_chunks[x_origin][z_origin + 1].get();

							loaded_chunks[x_origin][z_origin + 1]->edges.zm_chunk = loaded_chunks[x_origin][z_origin].get();

							//std::cout << "CHUNK UPDATE X: " << x_origin << " CHUNK Z: " << z_origin + 1 << std::endl;
							renderer.UpdateChunkMeshEdges(loaded_chunks[x_origin][z_origin + 1], 1.05f);
						}
						if (x > -render_distance) {
							//std::cout << x << std::endl;
							loaded_chunks[x_origin][z_origin]->edges.xm_chunk = loaded_chunks[x_origin - 1][z_origin].get();

							loaded_chunks[x_origin - 1][z_origin]->edges.xp_chunk = loaded_chunks[x_origin][z_origin].get();


							//std::cout << "CHUNK UPDATE X: " << x_origin - 1 << " CHUNK Z: " << z_origin << std::endl;
							renderer.UpdateChunkMeshEdges(loaded_chunks[x_origin - 1][z_origin], 1.05f);
						}


						renderer.SetupChunkMesh(loaded_chunks[x_origin][z_origin]);
						renderer.BuildChunkMesh(loaded_chunks[x_origin][z_origin], 1.25f);

					}

					center_origin.y = player_chunk_coordinates.y;
				}
				if (offset.x != 0) {
					// the player changed X chunk

					int32_t neg_x_origin = center_origin.x + (render_distance * -offset.x);
					int32_t x_origin = center_origin.x + ((render_distance + 1) * offset.x);

					for (int16_t z = -render_distance; z < render_distance + 1; z++)
					{

						int32_t z_origin = center_origin.y + z;
						loaded_chunks[neg_x_origin].erase(z_origin);

						//std::cout << x_origin << ", " << z_origin << std::endl;

						loaded_chunks[x_origin][z_origin] = std::make_unique<Chunk>(glm::ivec2{x_origin, z_origin}, seed);

						// might be able to optimize this if statement out, tried and failed 19/01/2024
						if (offset.x > 0) {
							// the player went X+ chunk

							loaded_chunks[x_origin][z_origin]->edges.xm_chunk = loaded_chunks[x_origin - 1][z_origin].get(); // changed to -1 from + 1

							loaded_chunks[x_origin - 1][z_origin]->edges.xp_chunk = loaded_chunks[x_origin][z_origin].get();

							//std::cout << "CHUNK UPDATE X: " << x_origin - 1 << " CHUNK Z: " << z_origin << std::endl;
							renderer.UpdateChunkMeshEdges(loaded_chunks[x_origin - 1][z_origin], 1.05f);
						}
						else {
							// the player went X- chunk

							loaded_chunks[x_origin][z_origin]->edges.xp_chunk = loaded_chunks[x_origin + 1][z_origin].get();

							loaded_chunks[x_origin + 1][z_origin]->edges.xm_chunk = loaded_chunks[x_origin][z_origin].get();
							//std::cout << "CHUNK UPDATE X: " << x_origin + 1 << " CHUNK Z: " << z_origin << std::endl;
							renderer.UpdateChunkMeshEdges(loaded_chunks[x_origin + 1][z_origin], 1.05f);
						}
						if (z > -render_distance) {
							loaded_chunks[x_origin][z_origin]->edges.zm_chunk = loaded_chunks[x_origin][z_origin - 1].get();

							//std::cout << z << std::endl;
							//std::cout << "z_origin: " << z_origin << std::endl;
							//std::cout << "-rend: " << -render_distance << std::endl;
							loaded_chunks[x_origin][z_origin - 1]->edges.zp_chunk = loaded_chunks[x_origin][z_origin].get();

							//std::cout << "CHUNK UPDATE X: " << x_origin << " CHUNK Z: " << z_origin - 1 << std::endl;
							renderer.UpdateChunkMeshEdges(loaded_chunks[x_origin][z_origin - 1], 1.05f);
						}


						renderer.SetupChunkMesh(loaded_chunks[x_origin][z_origin]);
						renderer.BuildChunkMesh(loaded_chunks[x_origin][z_origin], 1.25f);
					}

					loaded_chunks.erase(neg_x_origin);
					center_origin.x = player_chunk_coordinates.x;
				}
			}
		}


		renderer.RenderChunks(loaded_chunks);
	}

	glm::ivec2 ChunkLoader::CalculateChunkOriginFromGlobalPosition(glm::ivec2 pos) {
		return glm::ivec2(floor(pos.x / 16.0f), floor(pos.y / 16.0f));
		// 0.0 - 15.99 = 0, 16.0 - 31.99 = 1, 32.0 - 47.99 = 2
	}

	void ChunkLoader::RemoveBlock(glm::ivec3 pos) {
		glm::ivec2 chunk_origin = CalculateChunkOriginFromGlobalPosition(glm::ivec2(pos.x, pos.z));

		// finding the chunk the block belongs to
		if (auto it1 = loaded_chunks.find(chunk_origin.x); it1 != loaded_chunks.end()){
			if (auto it2 = it1->second.find(chunk_origin.y); it2 != it1->second.end()) {
				it2->second->blocks[Block3Dto1D(pos.x % 16, pos.y, pos.z % 16)] = BlockType::Air;
				renderer.BuildChunkMesh(it2->second, 1.0f);
			}
		}
	}

	void ChunkLoader::PlaceBlock(glm::ivec3 pos) {
		glm::ivec2 chunk_origin = CalculateChunkOriginFromGlobalPosition(glm::ivec2(pos.x, pos.z));
		// finding the chunk the block belongs to
		if (auto it1 = loaded_chunks.find(chunk_origin.x); it1 != loaded_chunks.end()) {
			if (auto it2 = it1->second.find(chunk_origin.y); it2 != it1->second.end()) {
				it2->second->blocks[Block3Dto1D(pos.x % 16, pos.y, pos.z % 16)] = BlockType::OakLog;
				renderer.BuildChunkMesh(it2->second, 1.0f);
			}
		}
	}

	ChunkRenderer::ChunkRenderer() {
		OpenGL::Shader vertex_chunk_shader(Util::ReadFile("resources/shaders/chunk_shader.vert").c_str(), OpenGL::ShaderType::VERTEX_SHADER);
		OpenGL::Shader fragment_chunk_shader(Util::ReadFile("resources/shaders/chunk_shader.frag").c_str(), OpenGL::ShaderType::FRAGMENT_SHADER);

		OpenGL::VertexLayout vertex_layout;
		vertex_layout.AddAttribute(OpenGL::VertexAttribute{1, 0, OpenGL::DataType::UNSIGNED_INT, OpenGL::DataTransformation::INT});

		chunk_shader = std::make_unique<OpenGL::ShaderProgram>(std::initializer_list<
			const OpenGL::Shader*>{ &vertex_chunk_shader, & fragment_chunk_shader }, vertex_layout);

		chunk_shader->SetUniformMatrix4FV("v_proj", 1, false, &Core::projection[0][0]);
	}

	void ChunkRenderer::RenderChunks(const std::unordered_map<int32_t, std::unordered_map<int32_t, std::unique_ptr<Chunk>>>& chunks) {
		chunk_shader->BindProgram();
		chunk_shader->SetUniformMatrix4FV("v_view", 1, false, &glm::inverse(Core::player_transform.GetTransform())[0][0]);

		for (const auto& [x, yMap] : chunks) {
			for (const auto& [z, chunk] : yMap) {

				// convert chunk coordinates to world coordinates
				glm::ivec2 origin = glm::ivec2(x * CHUNK_SIZE, z * CHUNK_SIZE);

				chunk_shader->SetUniform2IV("v_chunk_origin", 1, &origin[0]);

				// ssbo stuff

				//dummyVArray.Bind(); 	// needs to be added for SSBO type storage, on the backburner
				// draw num faces * 6


				chunk->mesh.v_array.Bind();
				chunk->mesh.v_array.DrawArrays(chunk->mesh.info);
			}

		}
	}
	// needs to be removed for SSBO type storage, on the backburner
	void ChunkRenderer::SetupChunkMesh(const std::unique_ptr<Chunk>& chunk) {

		chunk->mesh.v_array.SetVertexBuffer(chunk->mesh.v_buffer, chunk_shader->GetLayout(), 0, 0);

		chunk->mesh.info.first = 0;
		chunk->mesh.info.mode = OpenGL::DrawMode::TRIANGLES;
	}
	void ChunkRenderer::BuildChunkMesh(const std::unique_ptr<Chunk>& chunk, float allocation_multiplier) {
		std::vector<ChunkVertex> vertices;


		// iterate through chunk blocks on x, z, y
		for (uint8_t x = 0; x < CHUNK_SIZE; x++)
		{

			for (uint8_t z = 0; z < CHUNK_SIZE; z++)
			{
				for (uint16_t y = 0; y < CHUNK_HEIGHT; y++)
				{
					// get the block type at x, y, z within the chunk
					BlockType block_type = static_cast<BlockType>(chunk->blocks[Block3Dto1D(x, y, z)]);

					if (block_type != BlockType::Air)
					{
						// if block is not air, it will need faces rendered


						// top face
						if (y < CHUNK_HEIGHT - 1) {
							// a block exists at Y + 1

							if (chunk->blocks[Block3Dto1D(x, y + 1, z)] == BlockType::Air) {
								// the block at Y + 1 is air so a Y+ face needs to be generated

								uint32_t vertex = 0x00000000;

								vertex += static_cast<uint32_t>(block_type) << 22;

								vertex += static_cast<uint32_t>(x) << 17;
								vertex += static_cast<uint32_t>(y) << 8;
								vertex += static_cast<uint32_t>(z) << 3;

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
							}

						}
						else {
							// a block at Y + 1 does not exist so a top face needs to be generated

							uint32_t vertex = 0x00000000;

							vertex += static_cast<uint32_t>(block_type) << 22;

							vertex += static_cast<uint32_t>(x) << 17;
							vertex += static_cast<uint32_t>(y) << 8;
							vertex += static_cast<uint32_t>(z) << 3;

							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);

							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);
						}

						// bottom face
						if (y == 0 || chunk->blocks[Block3Dto1D(x, y - 1, z)] == BlockType::Air) {
							// the y is 0 so a Y- face needs to be generated or
							// the block at Y - 1 is air so a Y- face needs to be generated

							uint32_t vertex = 0x00000001;

							vertex += static_cast<uint32_t>(block_type) << 22;

							vertex += static_cast<uint32_t>(x) << 17;
							vertex += static_cast<uint32_t>(y) << 8;
							vertex += static_cast<uint32_t>(z) << 3;

							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);

							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);
						}

						// X+ face
						if (x < CHUNK_SIZE - 1) {
							// a block exists at X + 1
							if (chunk->blocks[Block3Dto1D(x + 1, y, z)] == BlockType::Air) {
								// the block at X + 1 is air so a X+ face needs to be generated

								uint32_t vertex = 0x00000002;

								vertex += static_cast<uint32_t>(block_type) << 22;

								vertex += static_cast<uint32_t>(x) << 17;
								vertex += static_cast<uint32_t>(y) << 8;
								vertex += static_cast<uint32_t>(z) << 3;

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
							}

						}
						else if (chunk->edges.xp_chunk != nullptr) {
							// a chunk exists at X+

							if (chunk->edges.xp_chunk->blocks[Block3Dto1D(0, y, z)] == BlockType::Air) {
								// the block at X+ in the next chunk is air so a X+ face needs to be generated.

								uint32_t vertex = 0x00000002;

								vertex += static_cast<uint32_t>(block_type) << 22;

								vertex += static_cast<uint32_t>(x) << 17;
								vertex += static_cast<uint32_t>(y) << 8;
								vertex += static_cast<uint32_t>(z) << 3;

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
							}

						}

						// X- face
						if (x > 0) {
							// a block exists at X - 1

							if (chunk->blocks[Block3Dto1D(x - 1, y, z)] == BlockType::Air) {
								// the block at X - 1 is air so a X- face needs to be generated

								uint32_t vertex = 0x00000003;

								vertex += static_cast<uint32_t>(block_type) << 22;

								vertex += static_cast<uint32_t>(x) << 17;
								vertex += static_cast<uint32_t>(y) << 8;
								vertex += static_cast<uint32_t>(z) << 3;

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
							}

						}
						else if (chunk->edges.xm_chunk != nullptr) {
							// a chunk exists at X-

							if (chunk->edges.xm_chunk->blocks[Block3Dto1D(CHUNK_SIZE - 1, y, z)] == BlockType::Air) {
								// the block at X- in the next chunk is air so a X- face needs to be generated.

								uint32_t vertex = 0x00000003;

								vertex += static_cast<uint32_t>(block_type) << 22;

								vertex += static_cast<uint32_t>(x) << 17;
								vertex += static_cast<uint32_t>(y) << 8;
								vertex += static_cast<uint32_t>(z) << 3;

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
							}

						}

						// Z+
						if (z < CHUNK_SIZE - 1) {
							// a block exists at Z + 1

							if (chunk->blocks[Block3Dto1D(x, y, z + 1)] == BlockType::Air) {
								// the block at Z + 1 is air so a Z+ face needs to be generated

								uint32_t vertex = 0x00000004;

								vertex += static_cast<uint32_t>(block_type) << 22;

								vertex += static_cast<uint32_t>(x) << 17;
								vertex += static_cast<uint32_t>(y) << 8;
								vertex += static_cast<uint32_t>(z) << 3;

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
							}

						}
						else if (chunk->edges.zp_chunk != nullptr) {
							// a chunk exists at Z+

							if (chunk->edges.zp_chunk->blocks[Block3Dto1D(x, y, 0)] == BlockType::Air) {
								// the block at Z+ in the next chunk is air so a Z+ face needs to be generated.

								uint32_t vertex = 0x00000004;

								vertex += static_cast<uint32_t>(block_type) << 22;

								vertex += static_cast<uint32_t>(x) << 17;
								vertex += static_cast<uint32_t>(y) << 8;
								vertex += static_cast<uint32_t>(z) << 3;

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
							}

						}

						// Z-
						if (z > 0) {
							// a block exists at Z - 1

							if (chunk->blocks[Block3Dto1D(x, y, z - 1)] == BlockType::Air) {
								// the block at Z - 1 is air so a Z- face needs to be generated

								uint32_t vertex = 0x00000005;

								vertex += static_cast<uint32_t>(block_type) << 22;

								vertex += static_cast<uint32_t>(x) << 17;
								vertex += static_cast<uint32_t>(y) << 8;
								vertex += static_cast<uint32_t>(z) << 3;

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
							}

						}
						else if (chunk->edges.zm_chunk != nullptr) {
							// a chunk exists at Z-

							if (chunk->edges.zm_chunk->blocks[Block3Dto1D(x, y, CHUNK_SIZE - 1)] == BlockType::Air) {
								// the block at Z- in the next chunk is air so a Z- face needs to be generated.

								uint32_t vertex = 0x00000005;

								vertex += static_cast<uint32_t>(block_type) << 22;

								vertex += static_cast<uint32_t>(x) << 17;
								vertex += static_cast<uint32_t>(y) << 8;
								vertex += static_cast<uint32_t>(z) << 3;

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
							}

						}
					}
				}

			}
		}

		if (chunk->mesh.v_buffer_size < vertices.size() * sizeof(ChunkVertex)) {
			// the buffer is too small for the extra data and an allocation needs to be done

			chunk->mesh.v_buffer.CreateMutableBuffer(static_cast<int64_t>(vertices.size() * sizeof(ChunkVertex) * allocation_multiplier), nullptr, OpenGL::BufferDataFlags::DYNAMIC_DRAW);
			chunk->mesh.v_buffer.SetBufferData(vertices.size() * sizeof(ChunkVertex), &vertices[0], 0);
			chunk->mesh.v_buffer_size = static_cast<uint32_t>(vertices.size() * sizeof(ChunkVertex) * allocation_multiplier);
			chunk->mesh.info.count = static_cast<int32_t>(vertices.size());
		}
		else {
			// the buffer is large enough for the extra data

			chunk->mesh.v_buffer.SetBufferData(vertices.size() * sizeof(ChunkVertex), &vertices[0], 0);
			chunk->mesh.info.count = static_cast<int32_t>(vertices.size());
		}

		chunk->mesh.info.count = static_cast<int32_t>(vertices.size());
	}
	void ChunkRenderer::BuildChunkMeshNoEdges(const std::unique_ptr<Chunk>& chunk, float allocation_multiplier) {
		std::vector<ChunkVertex> vertices;


		// iterate through chunk blocks on x, z, y
		for (uint8_t x = 0; x < CHUNK_SIZE; x++)
		{

			for (uint8_t z = 0; z < CHUNK_SIZE; z++)
			{
				for (uint16_t y = 0; y < CHUNK_HEIGHT; y++)
				{
					// get the block type at x, y, z within the chunk
					BlockType block_type = static_cast<BlockType>(chunk->blocks[Block3Dto1D(x, y, z)]);

					if (block_type != BlockType::Air)
					{
						// if block is not air, it will need faces rendered


						// top face
						if (y < CHUNK_HEIGHT - 1) {
							// a block exists at Y + 1

							if (chunk->blocks[Block3Dto1D(x, y + 1, z)] == BlockType::Air) {
								// the block at Y + 1 is air so a Y+ face needs to be generated

								uint32_t vertex = 0x00000000;

								vertex += static_cast<uint32_t>(block_type) << 22;

								vertex += static_cast<uint32_t>(x) << 17;
								vertex += static_cast<uint32_t>(y) << 8;
								vertex += static_cast<uint32_t>(z) << 3;

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
							}

						}
						else {
							// a block at Y + 1 does not exist so a top face needs to be generated

							uint32_t vertex = 0x00000000;

							vertex += static_cast<uint32_t>(block_type) << 22;

							vertex += static_cast<uint32_t>(x) << 17;
							vertex += static_cast<uint32_t>(y) << 8;
							vertex += static_cast<uint32_t>(z) << 3;

							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);

							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);
						}

						// bottom face
						if (y == 0 || chunk->blocks[Block3Dto1D(x, y - 1, z)] == BlockType::Air) {
							// the y is 0 so a Y- face needs to be generated or
							// the block at Y - 1 is air so a Y- face needs to be generated

							uint32_t vertex = 0x00000001;

							vertex += static_cast<uint32_t>(block_type) << 22;

							vertex += static_cast<uint32_t>(x) << 17;
							vertex += static_cast<uint32_t>(y) << 8;
							vertex += static_cast<uint32_t>(z) << 3;

							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);

							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);
						}

						// X+ face
						if (x < CHUNK_SIZE - 1) {
							// a block exists at X + 1
							if (chunk->blocks[Block3Dto1D(x + 1, y, z)] == BlockType::Air) {
								// the block at X + 1 is air so a X+ face needs to be generated

								uint32_t vertex = 0x00000002;

								vertex += static_cast<uint32_t>(block_type) << 22;

								vertex += static_cast<uint32_t>(x) << 17;
								vertex += static_cast<uint32_t>(y) << 8;
								vertex += static_cast<uint32_t>(z) << 3;

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
							}

						}

						// X- face
						if (x > 0) {
							// a block exists at X - 1

							if (chunk->blocks[Block3Dto1D(x - 1, y, z)] == BlockType::Air) {
								// the block at X - 1 is air so a X- face needs to be generated

								uint32_t vertex = 0x00000003;

								vertex += static_cast<uint32_t>(block_type) << 22;

								vertex += static_cast<uint32_t>(x) << 17;
								vertex += static_cast<uint32_t>(y) << 8;
								vertex += static_cast<uint32_t>(z) << 3;

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
							}

						}

						// Z+
						if (z < CHUNK_SIZE - 1) {
							// a block exists at Z + 1

							if (chunk->blocks[Block3Dto1D(x, y, z + 1)] == BlockType::Air) {
								// the block at Z + 1 is air so a Z+ face needs to be generated

								uint32_t vertex = 0x00000004;

								vertex += static_cast<uint32_t>(block_type) << 22;

								vertex += static_cast<uint32_t>(x) << 17;
								vertex += static_cast<uint32_t>(y) << 8;
								vertex += static_cast<uint32_t>(z) << 3;

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
							}

						}

						// Z-
						if (z > 0) {
							// a block exists at Z - 1

							if (chunk->blocks[Block3Dto1D(x, y, z - 1)] == BlockType::Air) {
								// the block at Z - 1 is air so a Z- face needs to be generated

								uint32_t vertex = 0x00000005;

								vertex += static_cast<uint32_t>(block_type) << 22;

								vertex += static_cast<uint32_t>(x) << 17;
								vertex += static_cast<uint32_t>(y) << 8;
								vertex += static_cast<uint32_t>(z) << 3;

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);

								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
								vertices.emplace_back(vertex);
							}

						}
					}
				}

			}
		}

		if (chunk->mesh.v_buffer_size < vertices.size() * sizeof(ChunkVertex)) {
			// the buffer is too small for the extra data and an allocation needs to be done

			chunk->mesh.v_buffer.CreateMutableBuffer(static_cast<int64_t>(vertices.size() * sizeof(ChunkVertex) * allocation_multiplier), nullptr, OpenGL::BufferDataFlags::DYNAMIC_DRAW);
			chunk->mesh.v_buffer.SetBufferData(vertices.size() * sizeof(ChunkVertex), &vertices[0], 0);
			chunk->mesh.v_buffer_size = static_cast<uint32_t>(vertices.size() * sizeof(ChunkVertex) * allocation_multiplier);
		}
		else {
			// the buffer is large enough for the extra data

			chunk->mesh.v_buffer.SetBufferData(vertices.size() * sizeof(ChunkVertex), &vertices[0], 0);
		}

		chunk->mesh.info.count = static_cast<int32_t>(vertices.size());
	}


	// chunk edge issue could definitely lie in here
	void ChunkRenderer::UpdateChunkMeshEdges(const std::unique_ptr<Chunk>& chunk, float allocation_multiplier) {

		std::vector<ChunkVertex> vertices;

		if (chunk->edges.xp_chunk != nullptr) {
			// if a chunk exists at X+

			//std::cout << "HAS X+ EDGE" << std::endl;

			// iterate over z, y
			for (uint8_t z = 0; z < CHUNK_SIZE; z++) {
				for (uint16_t y = 0; y < CHUNK_HEIGHT; y++) {
					BlockType primary_block_type = static_cast<BlockType>(chunk->blocks[Block3Dto1D(CHUNK_SIZE - 1, y, z)]);

					if (primary_block_type != BlockType::Air) {
						BlockType secondary_block_type = static_cast<BlockType>(chunk->edges.xp_chunk->blocks[Block3Dto1D(0, y, z)]);

						if (secondary_block_type == BlockType::Air) {

							uint32_t vertex = 0x00000002;

							vertex += static_cast<uint32_t>(primary_block_type) << 22;

							vertex += static_cast<uint32_t>(CHUNK_SIZE - 1) << 17;
							vertex += static_cast<uint32_t>(y) << 8;
							vertex += static_cast<uint32_t>(z) << 3;

							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);

							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);
						}
					}


				}
			}
		}
		if (chunk->edges.xm_chunk != nullptr) {
			// if a chunk exists at X-

			//std::cout << "HAS X- EDGE" << std::endl;

			// iterate over z, y
			for (uint8_t z = 0; z < CHUNK_SIZE; z++) {
				for (uint16_t y = 0; y < CHUNK_HEIGHT; y++) {
					BlockType primary_block_type = static_cast<BlockType>(chunk->blocks[Block3Dto1D(0, y, z)]);

					if (primary_block_type != BlockType::Air) {
						BlockType secondary_block_type = static_cast<BlockType>(chunk->edges.xm_chunk->blocks[Block3Dto1D(CHUNK_SIZE - 1, y, z)]);

						if (secondary_block_type == BlockType::Air) {

							uint32_t vertex = 0x00000003;

							vertex += static_cast<uint32_t>(primary_block_type) << 22;

							vertex += static_cast<uint32_t>(y) << 8;
							vertex += static_cast<uint32_t>(z) << 3;

							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);

							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);
						}
					}
				}
			}
		}
		if (chunk->edges.zp_chunk != nullptr) {
			// if a chunk exists at Z+

			//std::cout << "HAS Z+ EDGE" << std::endl;

			// iterate over x, y
			for (uint8_t x = 0; x < CHUNK_SIZE; x++) {
				for (uint16_t y = 0; y < CHUNK_HEIGHT; y++) {
					BlockType primary_block_type = static_cast<BlockType>(chunk->blocks[Block3Dto1D(x, y, CHUNK_SIZE - 1)]);

					if (primary_block_type != BlockType::Air) {
						BlockType secondary_block_type = static_cast<BlockType>(chunk->edges.zp_chunk->blocks[Block3Dto1D(x, y, 0)]);

						if (secondary_block_type == BlockType::Air) {

							uint32_t vertex = 0x00000004;

							vertex += static_cast<uint32_t>(primary_block_type) << 22;

							vertex += static_cast<uint32_t>(x) << 17;
							vertex += static_cast<uint32_t>(y) << 8;
							vertex += static_cast<uint32_t>(CHUNK_SIZE - 1) << 3;

							vertices.emplace_back(vertex); // replcae with bufferless rendering
							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);

							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);
						}
					}


				}
			}
		}
		if (chunk->edges.zm_chunk != nullptr) {
			// if a chunk exists at Z-

			//std::cout << "HAS Z- EDGE" << std::endl;

			// iterate over x, y
			for (uint8_t x = 0; x < CHUNK_SIZE; x++) {
				for (uint16_t y = 0; y < CHUNK_HEIGHT; y++) {
					BlockType primary_block_type = static_cast<BlockType>(chunk->blocks[Block3Dto1D(x, y, 0)]);

					if (primary_block_type != BlockType::Air) {
						BlockType secondary_block_type = static_cast<BlockType>(chunk->edges.zm_chunk->blocks[Block3Dto1D(x, y, CHUNK_SIZE - 1)]);

						if (secondary_block_type == BlockType::Air) {

							uint32_t vertex = 0x00000005;

							vertex += static_cast<uint32_t>(primary_block_type) << 22;

							vertex += static_cast<uint32_t>(x) << 17;
							vertex += static_cast<uint32_t>(y) << 8;

							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);

							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);
							vertices.emplace_back(vertex);




						}
					}


				}
			}

		}

		if (vertices.size() > 0) {


			uint32_t new_size = static_cast<uint32_t>(vertices.size() + chunk->mesh.info.count);

			if (chunk->mesh.v_buffer_size < new_size * sizeof(ChunkVertex)) {
				// the buffer is too small for the extra data and an allocation needs to be done
				// we need to regenerate the entire mesh and then add the edges

				chunk->mesh.v_buffer.CreateMutableBuffer(static_cast<int64_t>(new_size * sizeof(ChunkVertex) * allocation_multiplier), nullptr, OpenGL::BufferDataFlags::DYNAMIC_DRAW);

				chunk->mesh.v_buffer_size = static_cast<uint32_t>(new_size * sizeof(ChunkVertex) * allocation_multiplier);

				BuildChunkMeshNoEdges(chunk);

				chunk->mesh.v_buffer.SetBufferData(vertices.size() * sizeof(ChunkVertex), &vertices[0], chunk->mesh.info.count * sizeof(ChunkVertex));
			}
			else {
				// the buffer is large enough for the extra data

				chunk->mesh.v_buffer.SetBufferData(vertices.size() * sizeof(ChunkVertex), &vertices[0], chunk->mesh.info.count * sizeof(ChunkVertex));
			}

			chunk->mesh.info.count += static_cast<int32_t>(vertices.size());
		}
	}
}