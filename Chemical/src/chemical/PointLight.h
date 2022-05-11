#pragma once

#include <vendor/glm/glm.hpp>

#include "Transform.h"

namespace Chemical {

	struct PointLight {

		glm::vec3 diffuse = glm::vec3(0.0f);
		glm::vec3 ambient = glm::vec3(0.0f);
		glm::vec3 specular = glm::vec3(0.0f);

		Transform transform;
	};
}
