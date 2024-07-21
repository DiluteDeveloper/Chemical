#pragma once

#include <vector>

#include "vertex.h"

namespace Chemical {
	struct Mesh {
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) :
			vertices(vertices), indices(indices) {}

		Mesh() = default;
	};
}
