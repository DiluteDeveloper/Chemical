#pragma once

#include "transform.h"

class Camera {

	glm::vec2 oldCursorPos;

public:

	Transform transform;

	Camera(float speed = 5, float sensitivity = 5);
	void update();

	float speed = 5.0f;
	float sensitivity = 5.0f;
};
