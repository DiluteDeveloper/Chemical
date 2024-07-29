#pragma once

#include "util/transform.h"
#include "core/glfw_wrapper.h"

namespace Chemical {
	class Camera {
	public:
		Camera(const GLFWWrapper* wrapper);
		glm::mat4 UpdateMovement();

		bool GetEnabled();
		void SetEnabled(bool enabled);
	private:
		const GLFWWrapper* _wrapper;
		bool _enabled = true;

		Transform _transform;

		float _cam_speed = 0.05f;
		float _sensitivity = 0.08f;

		double _oldx = 0, _oldy = 0;
	};
}