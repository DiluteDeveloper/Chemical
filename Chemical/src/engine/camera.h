#pragma once

#include "transform.h"

class Camera {

	glm::vec2 oldCursorPos;

public:

	Transform transform;
	glm::mat4 proj;

	Camera(float speed = 5.0f, float sensitivity = 0.1f, float fov = 90.0f, float near = 0.1f, float far = 100.0f);
	void update();

	float speed = 5.0f;
	float sensitivity = 5.0f;
};
