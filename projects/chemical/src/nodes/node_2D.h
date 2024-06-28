#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Chemical {
	namespace Node {
		class Node2D {
		public:
			glm::fmat4 model_matrix = glm::mat4(1.0f);
			Node2D() = default;
			void set_position(float x, float y) {
				model_matrix = glm::translate(model_matrix, glm::vec3(x, y, 0));
			}
			void set_scale(float x, float y) {
				model_matrix = glm::scale(model_matrix, glm::vec3(x, y, 0));
			}
		};
	}
}
