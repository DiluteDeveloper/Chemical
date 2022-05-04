#pragma once

#include "transform.h"

#include <glm/gtc/matrix_transform.hpp>

Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) :
	position(position), rotation(rotation), scale(scale)
{

}

Transform::Transform(glm::vec3 position, glm::vec3 rotation) :
	position(position), rotation(rotation)
{

}

Transform::Transform(glm::vec3 position) :
	position(position)
{

}

glm::mat4 Transform::toMat4() const {
	glm::mat4 ret(1.0f);
	ret = glm::translate(ret, position);
	ret = glm::rotate(ret, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	ret = glm::rotate(ret, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	ret = glm::rotate(ret, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	ret = glm::scale(ret, scale);
	return ret;
}