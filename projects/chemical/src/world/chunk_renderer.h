#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "chunk.h"
#include "graphics/opengl/shader_program.h"

namespace Chemical {

	inline void RenderChunkGridMap(ChunkGridMap& grid_map, OpenGL::ShaderProgram& chunk_shader, const glm::mat4& view) {
		chunk_shader.BindProgram();
		chunk_shader.SetUniformMatrix4FV("v_view", 1, false, &view[0][0]);

		const auto& e = grid_map.GetGridMap();

		for (auto& [x, map] : e) {
			for (auto& [z, chunk] : map) {
				glm::ivec2 real_origin = glm::vec2(chunk.origin.x * CHUNK_SIZE, chunk.origin.z * CHUNK_SIZE);
				chunk_shader.SetUniform2IV("v_chunk_origin", 1, &real_origin[0]);
				chunk.BindAndDrawMesh();
			}
		}

	}
}