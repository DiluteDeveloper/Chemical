#pragma once

#include "graphics/opengl/vertex_array.h"
#include "graphics/opengl/shader_program.h"

#include <vector>
#include <glm/glm.hpp>

#define CHUNK_SIZE_X 200
#define CHUNK_SIZE_Z 200
#define CHUNK_BOTTOM -32

namespace Chemical {

	struct BlockData {
		glm::fvec3 colour = glm::fvec3(1.0f);
	};

	// could be uint16_t but padded to 32 anyway
	struct BlockID {
		uint32_t blockID = 0;

		operator uint32_t() const {
			return blockID;
		}

		BlockID(uint32_t blockID) : blockID(blockID) {}
		BlockID() {} // default constructor only needed for unordered_map use
	};

	// Getter and setter to avoid globals in a way
	void SetBlockData();
	const BlockData& GetBlockData(BlockID blockID);

	struct ChunkVertex {
		glm::fvec3 position;
		glm::fvec3 normal;

		ChunkVertex(const glm::fvec3& position, const glm::fvec3 normal) :
			position(position), normal(normal) {}
	};

	struct Chunk 
	{
		// x,z,z block origin is at -x-y-z
		std::array<std::array<std::unordered_map<int16_t, BlockID>, CHUNK_SIZE_Z>, CHUNK_SIZE_X> blocks;

		// chunk origin is at -x-z
		glm::dvec3 origin;
		Chunk(const glm::dvec3& origin, uint32_t seed);

	};

	struct ChunkRender {
		OpenGL::VertexArray vArray;
		OpenGL::ArrayDrawInfo info;

	};

	extern std::shared_ptr<ChunkRender> RenderChunk(const std::unique_ptr<Chunk>& chunk, const OpenGL::ShaderProgram& shader);

}