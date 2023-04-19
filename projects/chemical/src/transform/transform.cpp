#include "pch.h"
#include "transform.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Transform::GetTransform() const {
	glm::fmat4 rotationMat4 = glm::toMat4(glm::quat(rotation));

	return glm::translate(glm::fmat4(1.0f), position)
		* rotationMat4
		* glm::scale(glm::fmat4(1.0f), scale);
}