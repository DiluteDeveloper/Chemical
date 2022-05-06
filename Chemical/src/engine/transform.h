#pragma once

#include <glm/glm.hpp>

struct Transform {
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	Transform(glm::vec3 position, glm::vec3 rotation);

	Transform(glm::vec3 position);

	Transform() = default;

	glm::mat4 toMat4() const;

	bool operator==(const Transform& t) const { // might be fucked
		if (position != t.position)
			return false;
		if (rotation != t.rotation)
			return false;
		if (scale != t.scale)
			return false;
		return true;
	}

	bool operator!=(const Transform& t) const { // might be fucked
		if (position == t.position)
			return false;
		if (rotation == t.rotation)
			return false;
		if (scale == t.scale)
			return false;
		return true;
	}

};
