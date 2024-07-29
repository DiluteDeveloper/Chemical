#pragma once

#include "util/transform.h"
#include "core/glfw_wrapper.h"

namespace Chemical {
	class Camera {
	public:
		glm::mat4 UpdateMovement(const GLFWWrapper* wrapper);

	private:
		Transform transform;

		float camSpeed = 0.05f;
		float sensitivity = 0.08f;

		double oldx = 0, oldy = 0;
	};
}