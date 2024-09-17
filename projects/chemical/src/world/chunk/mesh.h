#pragma once

#include "graphics/opengl/vertex_array.h"
#include "block_grid.h"

namespace Chemical {
	typedef unsigned int ChunkVertex;

	inline OpenGL::VertexLayout GetChunkVertexLayout() {
		OpenGL::VertexLayout vertex_layout;
		vertex_layout.AddAttribute(OpenGL::VertexAttribute{ 1, 0, OpenGL::DataType::UNSIGNED_INT, OpenGL::DataTransformation::INT });
		return vertex_layout;
	}

	struct ChunkMesh {

		OpenGL::Buffer v_buffer;

		// the current byte allocation of v_buffer
		uint32_t v_buffer_size = 0;
		OpenGL::VertexArray v_array;
		OpenGL::ArrayDrawInfo info;

		ChunkMesh();
		void BuildMeshFromVertices(const std::vector<ChunkVertex>& vertices, float alloc_mult = 1.0f);
		void BindAndDraw() const;

	};


	extern void BuildChunkNorthBorderVertices(std::vector<ChunkVertex>& vertices, const ChunkBlockGrid& blocks, const ChunkBlockGrid& north_chunk);
	extern void BuildChunkEastBorderVertices(std::vector<ChunkVertex>& vertices, const ChunkBlockGrid& blocks, const ChunkBlockGrid& east_chunk);
	extern void BuildChunkSouthBorderVertices(std::vector<ChunkVertex>& vertices, const ChunkBlockGrid& blocks, const ChunkBlockGrid& south_chunk);
	extern void BuildChunkWestBorderVertices(std::vector<ChunkVertex>& vertices, const ChunkBlockGrid& blocks, const ChunkBlockGrid& west_chunk);

	extern std::vector<ChunkVertex> BuildChunkVertices(const ChunkBlockGrid& blocks);
}