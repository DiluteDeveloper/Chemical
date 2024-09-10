#pragma once

#include "graphics/opengl/buffer.h"
#include "graphics/opengl/vertex_array.h"
#include "block.h"

// limit of 255 due to uint8_t iterators.
// determines the X and Z dimensions of every chunk.
// do not change without modifying shader code.
constexpr uint8_t CHUNK_SIZE = 16;

// no easily reachable limit.
// determines the y dimension of a chunk used for iterating over.
// do not change without modifying shader code.
constexpr uint16_t CHUNK_HEIGHT = 400;

namespace Chemical {

	// converts 3D block coordinates to 1D
	inline uint32_t Block3Dto1D(uint8_t x, uint16_t y, uint8_t z) {
		return ((y * CHUNK_SIZE * CHUNK_SIZE) + (z * CHUNK_SIZE) + x);
	}

	typedef std::array<BlockType, (CHUNK_SIZE* CHUNK_SIZE)* CHUNK_HEIGHT> ChunkBlockData;

	struct ChunkData {
		ChunkBlockData blocks = { BlockType::Air };

		// North, East, South, West, never eat soggy weetbix
		std::array<ChunkData*, 4> edges = { nullptr };
	};

	typedef std::array<std::array<int16_t, CHUNK_SIZE>, CHUNK_SIZE> ChunkHeightMapData;

	extern void GenerateTerrainChunkBlockData(ChunkBlockData& block_data, const ChunkHeightMapData& height_map);

	struct ChunkMeshData {

		OpenGL::Buffer v_buffer;

		// the current byte allocation of v_buffer
		uint32_t v_buffer_size = 0;
		OpenGL::VertexArray v_array;
		OpenGL::ArrayDrawInfo info;

	};

	// will only generate edge faces if they exist and generate_chunk_edge_faces is true
	extern ChunkMeshData GenerateChunkMeshData(const ChunkData& chunk_data, bool generate_chunk_edge_faces, float allocation_multiplier = 1.0f);

	extern void UpdateChunkMeshEdgeData(ChunkMeshData& mesh_data, const ChunkData& chunk_data, float allocation_multiplier = 1.0f);

	inline OpenGL::VertexLayout GetChunkVertexLayout() {
		OpenGL::VertexLayout vertex_layout;
		vertex_layout.AddAttribute(OpenGL::VertexAttribute{ 1, 0, OpenGL::DataType::UNSIGNED_INT, OpenGL::DataTransformation::INT });
		return vertex_layout;
	}
}
