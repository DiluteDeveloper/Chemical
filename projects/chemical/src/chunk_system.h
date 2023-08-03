#pragma once

#include "graphics/opengl/vertex_array.h"
#include "graphics/opengl/shader_program.h"

#include <vector>
#include <glm/glm.hpp>

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Z 16
#define CHUNK_BOTTOM -32

namespace Chemical {

	struct Block {
		glm::fvec3 colour = glm::fvec3(1.0f);

		Block(const glm::fvec3& colour) : colour(colour) {}
		Block() {}
	};

	struct ChunkVertex {
		glm::fvec3 position;
		glm::fvec3 normal;

		ChunkVertex(const glm::fvec3& position, const glm::fvec3 normal) :
			position(position), normal(normal) {}
	};

	// chunk blocks will iterate along chunk x then z
	// block origin is at -x-y-z
	struct Chunk 
	{
		// position maps to a block
		std::unordered_map<int16_t, Block> blocks[CHUNK_SIZE_X][CHUNK_SIZE_Z];

		glm::dvec3 origin;
		Chunk(const glm::dvec3& origin);
	};

	struct ChunkRender {
		OpenGL::VertexArray vArray;
		OpenGL::ArrayDrawInfo info;

	};

	extern std::shared_ptr<ChunkRender> RenderChunk(Chunk& chunk, const OpenGL::ShaderProgram& shader);

}