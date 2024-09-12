#pragma once

#include "graphics/opengl/buffer.h"
#include "graphics/opengl/vertex_array.h"

#include "chunk_grid.h"

// limit of 32 due to bit data system
// determines the X and Z dimensions of every chunk.
// do not change without modifying shader code.
constexpr uint8_t CHUNK_SIZE = 16;


namespace Chemical {

	typedef uint32_t ChunkVertex;

	struct ChunkGrid {
	public:

		ChunkGrid();

		ChunkGridOrigin origin;
		ChunkBlockGrid blocks;

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
