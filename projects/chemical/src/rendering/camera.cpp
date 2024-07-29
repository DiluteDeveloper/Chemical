#include <pch.h>
#include <glm/gtc/matrix_transform.hpp>
#include "util/transform.h"
#include "camera.h"
#include "core/glfw_wrapper.h"


namespace Chemical {

	Camera::Camera(const GLFWWrapper* wrapper) :
		_wrapper(wrapper) {}

	glm::mat4 Camera::UpdateMovement() {

		glm::fmat4 matrix = _transform.TransformToMat4();

		if (_enabled) {


			glm::fvec3 right = matrix[0];
			glm::fvec3 up = matrix[1];
			glm::fvec3 forward = matrix[2];
			glm::fvec3 front = glm::normalize(glm::fvec3(forward.x, 0, forward.z));

			GLFWwindow* window = _wrapper->GetGLFWWindow();
			if (glfwGetKey(window, GLFW_KEY_W)) {
				_transform.position += front * _cam_speed;
			}
			if (glfwGetKey(window, GLFW_KEY_S)) {
				_transform.position -= front * _cam_speed;
			}
			if (glfwGetKey(window, GLFW_KEY_A)) {
				_transform.position -= right * _cam_speed;
			}
			if (glfwGetKey(window, GLFW_KEY_D)) {
				_transform.position += right * _cam_speed;
			}
			if (glfwGetKey(window, GLFW_KEY_SPACE)) {
				_transform.position.y += _cam_speed;
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
				_transform.position.y -= _cam_speed;
			}

			double x, y;
			glfwGetCursorPos(window, &x, &y);

			_transform.rotation.y += static_cast<float>(x - _oldx) * _sensitivity;
			_transform.rotation.x += static_cast<float>(y - _oldy) * _sensitivity;

			_transform.rotation.x = glm::clamp(_transform.rotation.x, -85.0f, 85.0f);

			_oldx = x;
			_oldy = y;
		}

		return matrix;
	}

	bool Camera::GetEnabled() {
		return _enabled;
	}
	void Camera::SetEnabled(bool enabled) {
		_enabled = enabled;
		if (enabled) {
			glfwGetCursorPos(_wrapper->GetGLFWWindow(), &_oldx, &_oldy);
		}
	}

}