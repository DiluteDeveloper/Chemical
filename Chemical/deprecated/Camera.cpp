#include "Camera.h"
#include "core/Window.h"
#include "Time.h"

#include <vendor/glm/glm.hpp>
#include <vendor/glm/gtc/matrix_transform.hpp>

namespace Chemical {
	Camera::Camera(float speed, float sensitivity, float fov, float near, float far) :
		speed(speed), sensitivity(sensitivity)
	{
		glm::vec2 windowSize = Core::Window::GetSize();
		proj = glm::perspective(glm::radians(fov), (float)windowSize.x / (float)windowSize.y, near, far);
		oldCursorPos = Core::Window::GetCursorPos();
	}
	void Camera::Update() {
		glm::fmat4 matrix = transform.ToMat4();
		glm::fvec3 forward = matrix[2];
		glm::fvec3 front = glm::normalize(glm::vec3(forward.x, 0, forward.z));
		glm::fvec3 right = matrix[0];

		if (Core::Window::GetKey(GLFW_KEY_W)) {
			transform.position -= front * (float)Time::deltaTime * speed;
		}
		if (Core::Window::GetKey(GLFW_KEY_S)) {
			transform.position += front * (float)Time::deltaTime * speed;
		}
		if (Core::Window::GetKey(GLFW_KEY_A)) {
			transform.position -= right * (float)Time::deltaTime * speed;
		}
		if (Core::Window::GetKey(GLFW_KEY_D)) {
			transform.position += right * (float)Time::deltaTime * speed;
		}
		if (Core::Window::GetKey(GLFW_KEY_SPACE)) {
			transform.position.y += (float)Time::deltaTime * speed;
		}
		if (Core::Window::GetKey(GLFW_KEY_LEFT_SHIFT)) {
			transform.position.y -= (float)Time::deltaTime * speed;
		}

		glm::ivec2 cursorPos = Core::Window::GetCursorPos();

		transform.rotation.y -= (cursorPos.x - oldCursorPos.x) * sensitivity;
		transform.rotation.x -= (cursorPos.y - oldCursorPos.y) * sensitivity;

		transform.rotation.x = glm::clamp(transform.rotation.x, -80.0f, 80.0f);

		oldCursorPos = cursorPos;
	}
}
