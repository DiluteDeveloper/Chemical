#include <pch.h>

#include "chunk.h"

namespace Chemical {

	// converts 3D block coordinates to 1D
	//inline uint32_t Block3Dto1D(uint8_t x, uint16_t y, uint8_t z) {
	//	return ((y * CHUNK_SIZE * CHUNK_SIZE) + (z * CHUNK_SIZE) + x);
	//}

	Chunk const* ChunkGridMap::GetChunk(ChunkGridOrigin origin) const {
		if (grid_map.contains(origin.x))
			if (grid_map.at(origin.x).contains(origin.z))
				return &grid_map.at(origin.x).at(origin.z);
		return nullptr;
	}

	Chunk& ChunkGridMap::CreateChunk(ChunkGridOrigin origin) {
		Chunk& chunk = grid_map[origin.x][origin.z];
		chunk.origin = origin;
		return chunk;
	}

	std::unordered_map<int, std::unordered_map<int, Chunk>>& ChunkGridMap::GetGridMap() {
		return grid_map;
	}

	void Chunk::GenerateTerrainFromHeightMap(const ChunkHeightMap& height_map) {

		for (uint8_t x = 0; x < CHUNK_SIZE; x++)
		{
			for (uint8_t z = 0; z < CHUNK_SIZE; z++)
			{
				unsigned int height = height_map[x][z];

				// reverse iterator to go from terrain height - 0(bottom y of the chunk)
				// If genHeight is past CHUNK_HEIGHT it will be empty past that height
				for (int64_t y = height; y >= 0; y--)
				{

					if (y == 0) {
						blocks.SetBlockType(BlockType::Bedrock, x, y, z);
						continue;
					}


					if (y <= height - 1) {
						if (y <= height - 4)
							blocks.SetBlockType(BlockType::Stone, x, y, z);
						else
							blocks.SetBlockType(BlockType::Dirt, x, y, z);
					}
					else
						blocks.SetBlockType(BlockType::Grass, x, y, z);
				}
			}
		}
	}

	Chunk::Chunk(ChunkGridOrigin origin) : origin(origin) {
		v_array.SetVertexBuffer(v_buffer, GetChunkVertexLayout(), 0, 0);

		info.first = 0;
		info.mode = OpenGL::DrawMode::TRIANGLES;
	}

	void Chunk::BuildChunkBorderVertices(std::vector<ChunkVertex>& vertices, const ChunkGridMap& grid_map) {

		Chunk const* east_chunk = grid_map.GetChunk(ChunkGridOrigin{ origin.x + 1, origin.z });
		if (east_chunk != nullptr) {
			// if a chunk exists at X+

			//std::cout << "HAS X+ EDGE" << std::endl;

			// iterate over z, y
			for (uint8_t z = 0; z < CHUNK_SIZE; z++) {
				unsigned int max_height = blocks.GetMaxHeight(CHUNK_SIZE - 1, z);
				for (unsigned int y = 0; y <= max_height; y++) {
					BlockType primary_block_type = blocks.GetBlockType(CHUNK_SIZE - 1, y, z);

					if (primary_block_type != BlockType::Air) {
						BlockType secondary_block_type = east_chunk->blocks.GetBlockType(0, y, z);

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
		Chunk const* west_chunk = grid_map.GetChunk(ChunkGridOrigin{ origin.x - 1, origin.z });
		if (west_chunk != nullptr) {
			// if a chunk exists at X-

			//std::cout << "HAS X- EDGE" << std::endl;

			// iterate over z, y
			for (uint8_t z = 0; z < CHUNK_SIZE; z++) {
				unsigned int max_height = blocks.GetMaxHeight(0, z);
				for (unsigned int y = 0; y <= max_height; y++) {
					BlockType primary_block_type = blocks.GetBlockType(0, y, z);

					if (primary_block_type != BlockType::Air) {
						BlockType secondary_block_type = west_chunk->blocks.GetBlockType(CHUNK_SIZE - 1, y, z);

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
		Chunk const* south_chunk = grid_map.GetChunk(ChunkGridOrigin{ origin.x, origin.z + 1});
		if (south_chunk != nullptr) {
			// if a chunk exists at Z+

			//std::cout << "HAS Z+ EDGE" << std::endl;

			// iterate over x, y
			for (uint8_t x = 0; x < CHUNK_SIZE; x++) {
				unsigned int max_height = blocks.GetMaxHeight(x, CHUNK_SIZE - 1);
				for (unsigned int y = 0; y <= max_height; y++) {
					BlockType primary_block_type = blocks.GetBlockType(x, y, CHUNK_SIZE - 1);

					if (primary_block_type != BlockType::Air) {
						BlockType secondary_block_type = south_chunk->blocks.GetBlockType(x, y, 0);

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
		Chunk const* north_chunk = grid_map.GetChunk(ChunkGridOrigin{ origin.x, origin.z - 1});
		if (north_chunk != nullptr) {
			// if a chunk exists at Z-

			//std::cout << "HAS Z- EDGE" << std::endl;

			// iterate over x, y
			for (uint8_t x = 0; x < CHUNK_SIZE; x++) {
				unsigned int max_height = blocks.GetMaxHeight(x, 0);
				for (unsigned int y = 0; y <= max_height; y++) {
					BlockType primary_block_type = blocks.GetBlockType(x, y, 0);

					if (primary_block_type != BlockType::Air) {
						BlockType secondary_block_type = north_chunk->blocks.GetBlockType(x, y, CHUNK_SIZE - 1);

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
	}

	std::vector<ChunkVertex> Chunk::BuildVertices() {

		std::vector<ChunkVertex> vertices;


		// iterate through chunk blocks on x, z, y
		for (uint8_t x = 0; x < CHUNK_SIZE; x++)
		{

			for (uint8_t z = 0; z < CHUNK_SIZE; z++)
			{
				unsigned int max_height = blocks.GetMaxHeight(x, z);
				for (unsigned int y = 0; y <= max_height; y++)
				{
					// get the block type at x, y, z within the chunk
					BlockType block_type = static_cast<BlockType>(blocks.GetBlockType(x, y, z));

					if (block_type != BlockType::Air)
					{
						// if block is not air, it will need faces rendered


						if (blocks.GetBlockType(x, y + 1, z) == BlockType::Air) {
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

						// bottom face
						if (y == 0 || blocks.GetBlockType(x, y - 1, z) == BlockType::Air) {
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
							if (blocks.GetBlockType(x + 1, y, z) == BlockType::Air) {
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

							if (blocks.GetBlockType(x - 1, y, z) == BlockType::Air) {
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

							if (blocks.GetBlockType(x, y, z + 1) == BlockType::Air) {
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

							if (blocks.GetBlockType(x, y, z - 1) == BlockType::Air) {
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
		return vertices;
	}

	void Chunk::BuildMeshFromVertices(const std::vector<ChunkVertex>& vertices, float alloc_mult) {
		v_buffer.CreateMutableBuffer(static_cast<int64_t>(vertices.size() * sizeof(ChunkVertex) * alloc_mult), nullptr, OpenGL::BufferDataFlags::DYNAMIC_DRAW);
		v_buffer.SetBufferData(vertices.size() * sizeof(ChunkVertex), &vertices[0], 0);
		v_buffer_size = static_cast<uint32_t>(vertices.size() * sizeof(ChunkVertex) * alloc_mult);

		info.count = static_cast<int32_t>(vertices.size());

	}

	void Chunk::BindAndDrawMesh() const {
		v_array.Bind();
		v_array.DrawArrays(info);
	}
}