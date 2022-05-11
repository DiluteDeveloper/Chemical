#pragma once

#include <vendor/glm/glm.hpp>

namespace Chemical {
	struct Material {

		glm::fvec3 diffuse = glm::fvec3(1.0f);
		glm::fvec3 specular = glm::fvec3(1.0f);
		glm::fvec3 ambient = glm::fvec3(1.0f);
		float shininess = 32.0f;

		std::string filePath = "";

		Material(const std::string& filePath) : filePath(filePath) {}
	};

}
