#pragma once

#include "graphics/opengl/vertex_array.h"
#include "graphics/opengl/shader_program.h"

#include <vector>
#include <glm/glm.hpp>

#define CHUNK_SIZE_X 128
#define CHUNK_SIZE_Z 128
#define CHUNK_BOTTOM -64

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

	// block origin is at -x-y-z
	struct Chunk 
	{
		// x,z,y
		std::array<std::array<std::unordered_map<int16_t, Block>, CHUNK_SIZE_Z>, CHUNK_SIZE_X> blocks;

		glm::dvec3 origin;
		Chunk(const glm::dvec3& origin);

		void RemoveBlock(glm::ivec3 position);
		void AddBlock(glm::i16vec3 position, glm::fvec3 colour);
	};

	struct ChunkRender {
		OpenGL::VertexArray vArray;
		OpenGL::ArrayDrawInfo info;

	};

	extern std::shared_ptr<ChunkRender> RenderChunk(Chunk& chunk, const OpenGL::ShaderProgram& shader);

}