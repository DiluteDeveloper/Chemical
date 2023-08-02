#pragma once

#include "graphics/opengl/vertex_array.h"
#include "graphics/opengl/shader_program.h"

#include <vector>
#include <glm/glm.hpp>

#define CHUNK_SIZE_X 80
#define CHUNK_SIZE_Z 400

// chunk blocks will iterate along chunk x then z
struct Chunk {
	std::vector<int> heightMap;
	glm::dvec3 origin;
	Chunk(const glm::dvec3& origin) : origin(origin) {}

	void GenerateHeightmap();
};

extern OpenGL::VertexArray RenderChunk(Chunk& chunk, const OpenGL::ShaderProgram& shader);
