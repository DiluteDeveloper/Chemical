#pragma once

#include "graphics/opengl/buffer.h"
#include "graphics/opengl/vertex_array.h"
#include "block.h"

// limit of 32 due to bit data system
// determines the X and Z dimensions of every chunk.
// do not change without modifying shader code.
constexpr uint8_t CHUNK_SIZE = 16;

// no easily reachable limit.
// determines the y dimension of a chunk used for iterating over.
// do not change without modifying shader code.
constexpr uint16_t CHUNK_HEIGHT = 400;

namespace Chemical {

	// Chunk origin coordinates in chunk space
	// aka x * CHUNK_SIZE & z * CHUNK_SIZE
	struct ChunkGridOrigin {
		int x = 0;
		int z = 0;

		ChunkGridOrigin(int x = 0, int z = 0) :
			x(x), z(z) {}
	};

	class Chunk;
	typedef std::array<BlockType, (CHUNK_SIZE* CHUNK_SIZE)* CHUNK_HEIGHT> ChunkBlockArray;
	typedef std::array<std::array<int16_t, CHUNK_SIZE>, CHUNK_SIZE> ChunkHeightMap;
	typedef uint32_t ChunkVertex;

	class ChunkGridMap {
	public:
		// returns nullptr if chunk doesnt exist at origin
		Chunk const* GetChunk(ChunkGridOrigin origin) const;

		// returns existing chunk if already exists
		Chunk& CreateChunk(ChunkGridOrigin origin);

		std::unordered_map<int, std::unordered_map<int, Chunk>>& GetGridMap();

	private:
		std::unordered_map<int, std::unordered_map<int, Chunk>> grid_map;
	};


	class Chunk {
	public:

		Chunk(ChunkGridOrigin origin = {});

		// Fills in the block chunk block array with terrain using height map
		void GenerateTerrainFromHeightMap(const ChunkHeightMap& height_map);

		// Builds the mesh from the block data created by GenerateTerrainFromHeightMap
		std::vector<ChunkVertex> BuildVertices();

		// Using ChunkGridMap adds faces to the mesh that are on the edge of two chunks
		void BuildChunkBorderVertices(std::vector<ChunkVertex>& vertices, const ChunkGridMap& grid_map);

		void BuildMeshFromVertices(const std::vector<ChunkVertex>& vertices, float alloc_mult = 1.0f);

		void BindAndDrawMesh() const;

		ChunkGridOrigin origin;


	private:
		std::unique_ptr<ChunkBlockArray> blocks = std::make_unique<ChunkBlockArray>();

		// I hate the syntax for accessing [] operator on unique_ptr so this is the solution
		ChunkBlockArray& GetBlocks() const;

		OpenGL::Buffer v_buffer;

		// the current byte allocation of v_buffer
		uint32_t v_buffer_size = 0;
		OpenGL::VertexArray v_array;
		OpenGL::ArrayDrawInfo info;

	};

	inline OpenGL::VertexLayout GetChunkVertexLayout() {
		OpenGL::VertexLayout vertex_layout;
		vertex_layout.AddAttribute(OpenGL::VertexAttribute{ 1, 0, OpenGL::DataType::UNSIGNED_INT, OpenGL::DataTransformation::INT });
		return vertex_layout;
	}
}
