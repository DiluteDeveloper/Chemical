#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "grid.h"
#include "graphics/opengl/shader_program.h"

namespace Chemical {

	inline void RenderChunkGrid(const std::unique_ptr<ChunkGrid>& grid, OpenGL::ShaderProgram& chunk_shader, const glm::mat4& view) {
		chunk_shader.BindProgram();
		chunk_shader.SetUniformMatrix4FV("v_view", 1, false, &view[0][0]);

		for (auto& [x, map] : grid->chunk_meshes) {
			for (auto& [z, mesh] : map) {
				glm::ivec2 real_origin = glm::vec2(x * CHUNK_SIZE, z * CHUNK_SIZE);
				chunk_shader.SetUniform2IV("v_chunk_origin", 1, &real_origin[0]);
				mesh.BindAndDraw();
			}
		}

	}
}