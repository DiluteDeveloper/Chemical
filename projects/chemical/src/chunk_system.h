#pragma once

#include "graphics/opengl/vertex_array.h"
#include "graphics/opengl/shader_program.h"

#include <vector>
#include <glm/glm.hpp>

#define CHUNK_SIZE_X 480
#define CHUNK_SIZE_Z 720

// chunk blocks will iterate along chunk x then z
struct Chunk {
	std::vector<int> heightMap;
	glm::dvec3 origin;
	Chunk(const glm::dvec3& origin) : origin(origin) {}

	void GenerateHeightmap();
};

struct ChunkRender {
	OpenGL::VertexArray vArray;
	OpenGL::ArrayDrawInfo info;

};

extern std::shared_ptr<ChunkRender> RenderChunk(Chunk& chunk, const OpenGL::ShaderProgram& shader);
