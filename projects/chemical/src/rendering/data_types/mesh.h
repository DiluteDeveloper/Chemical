#pragma once

#include <vector>

#include "vertex.h"

#include "../opengl/vertex_array.h"
#include "util/transform.h"

namespace Chemical {

	// Needs to be constructed as a shared_ptr
	struct Mesh {
		friend std::shared_ptr<Mesh> std::make_shared<Mesh>();

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		OpenGL::VertexArray v_array;
		OpenGL::ElementDrawInfo draw_info;

		Transform transform;

		void GenerateMesh();

		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) :
			vertices(vertices), indices(indices) {}

		Mesh() = default;

	};
}
