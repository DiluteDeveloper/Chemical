#pragma once

#include "graphics/opengl/vertex_array.h"
#include "graphics/opengl/shader_program.h"

#include <vector>
#include <glm/glm.hpp>

#define CHUNK_SIZE_X 320
#define CHUNK_SIZE_Y 248
#define CHUNK_SIZE_Z 320

namespace Chemical {


	void InitializeBlockData(); // needs to be called before any chunk generation code

	using BlockID = uint16_t;

	struct Chunk 
	{
		// x,z,z block origin is at -x-y-z
		std::array<BlockID, CHUNK_SIZE_X* CHUNK_SIZE_Y* CHUNK_SIZE_Z> blocks = { 0 };

		// chunk origin is at -x-z
		glm::dvec3 origin;
		Chunk(const glm::dvec3& origin, uint32_t seed);

	};

	struct ChunkVertex {
		glm::fvec3 position;
		glm::fvec3 normal;

		ChunkVertex(const glm::fvec3& position, const glm::fvec3 normal) :
			position(position), normal(normal) {}
	};

	struct ChunkMesh {
		OpenGL::VertexArray v_array;
		OpenGL::ArrayDrawInfo info;

	};

	extern std::shared_ptr<ChunkMesh> RenderChunk(const std::unique_ptr<Chunk>& chunk, const OpenGL::ShaderProgram& shader);

}