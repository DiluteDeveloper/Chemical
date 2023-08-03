#pragma once

#include "graphics/opengl/vertex_array.h"
#include "graphics/opengl/shader_program.h"

#include <vector>
#include <glm/glm.hpp>

#define CHUNK_SIZE_X 128
#define CHUNK_SIZE_Z 128

/*struct BlockInstance {
	glm::ivec3 position; // Position could be i8vec3 but would limit chunk size to 256x256
	// X type/blockid;

	BlockInstance(const glm::ivec3& position) : position(position) {}
};*/


struct ChunkVertex {
	glm::fvec3 position;
	glm::fvec3 normal;

	ChunkVertex(const glm::fvec3& position, const glm::fvec3 normal) :
		position(position), normal(normal) {}
};

// chunk blocks will iterate along chunk x then z
struct Chunk {

	std::vector<int16_t> visibleBlocks[CHUNK_SIZE_X][CHUNK_SIZE_Z];
	//outer vector is X, inner vector is Z, value is Y
	//std::vector<std::vector<int16_t>> visibleBlocks;
	glm::dvec3 origin;
	Chunk(const glm::dvec3& origin);
};

struct ChunkRender {
	OpenGL::VertexArray vArray;
	OpenGL::ArrayDrawInfo info;

};

extern std::shared_ptr<ChunkRender> RenderChunk(Chunk& chunk, const OpenGL::ShaderProgram& shader);
