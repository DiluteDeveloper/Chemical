#pragma once

#include "graphics/opengl/vertex_array.h"
#include "graphics/opengl/shader_program.h"

#include <vector>
#include <glm/glm.hpp>

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Z 16
#define CHUNK_BOTTOM -60

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

	struct Chunk 
	{
		// x,z,z block origin is at -x-y-z
		std::array<std::array<std::unordered_map<int16_t, Block>, CHUNK_SIZE_Z>, CHUNK_SIZE_X> blocks;

		// chunk origin is at -x-z
		glm::dvec2 origin;
		Chunk(const glm::dvec2& origin, uint32_t seed);

	};

	struct ChunkRender {
		OpenGL::VertexArray vArray;
		OpenGL::ArrayDrawInfo info;

	};

	extern std::shared_ptr<ChunkRender> RenderChunk(Chunk& chunk, const OpenGL::ShaderProgram& shader);

}