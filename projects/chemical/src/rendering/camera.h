#pragma once

#include "util/transform.h"

namespace Chemical {
	class Camera {
	public:
		glm::mat4 UpdateMovement();

	private:
		Transform transform;

		float camSpeed = 0.05f;
		float sensitivity = 0.08f;

		double oldx = 0, oldy = 0;
	};
}