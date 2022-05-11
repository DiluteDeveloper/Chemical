#pragma once

#include "Transform.h"

#include <vendor/glm/gtc/matrix_transform.hpp>

namespace Chemical {
	Transform::Transform(glm::fvec3 position, glm::fvec3 rotation, glm::fvec3 scale) :
		position(position), rotation(rotation), scale(scale) {}

	glm::fmat4 Transform::ToMat4() const {
		glm::fmat4 ret(1.0f);
		ret = glm::translate(ret, position);
		ret = glm::rotate(ret, glm::radians(rotation.z), glm::fvec3(0, 0, 1));
		ret = glm::rotate(ret, glm::radians(rotation.y), glm::fvec3(0, 1, 0));
		ret = glm::rotate(ret, glm::radians(rotation.x), glm::fvec3(1, 0, 0));
		ret = glm::scale(ret, scale);
		return ret;
	}
}
