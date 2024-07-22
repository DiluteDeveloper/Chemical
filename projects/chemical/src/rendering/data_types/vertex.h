#pragma once

#include <glm/glm.hpp>

#include "opengl/misc.h"

namespace Chemical {

	struct Vertex {
		inline static OpenGL::VertexLayout vertex_layout;

		static void InitializeVertexLayout() {
			vertex_layout.AddAttribute(OpenGL::VertexAttribute(3, 0));
			vertex_layout.AddAttribute(OpenGL::VertexAttribute(3, 3 * sizeof(float)));
		}

		glm::vec3 position;
		glm::vec3 normal;

		Vertex(const glm::vec3& position, const glm::vec3& normal) :
			position(position), normal(normal) {}

		Vertex() = default;
	};
}
