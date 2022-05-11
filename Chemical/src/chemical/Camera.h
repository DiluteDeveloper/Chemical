#pragma once

#include "Transform.h"

namespace Chemical {
	class Camera {

		glm::ivec2 oldCursorPos;

	public:

		Transform transform;
		glm::fmat4 proj;

		Camera(float speed = 5.0f, float sensitivity = 0.1f, float fov = 90.0f, float near = 0.1f, float far = 100.0f);
		void Update();

		float speed = 5.0f;
		float sensitivity = 0.1f;
	};

}
