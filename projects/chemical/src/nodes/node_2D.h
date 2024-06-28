#pragma once

#include <glm/glm.hpp>

namespace Chemical {
	namespace Node {
		class Node2D {
		public:
			glm::fvec2 position = glm::fvec2(0.0f);
			glm::fvec2 scale = glm::fvec2(1.0f);
		};
	}
}
