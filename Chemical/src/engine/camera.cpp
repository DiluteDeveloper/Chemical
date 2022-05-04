#include "camera.h"
#include "window.h"
#include "time.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

Camera::Camera(float speed, float sensitivity) : speed(speed), sensitivity(sensitivity) {
	oldCursorPos = Window::getCursorPos();
}
void Camera::update() {
	glm::mat4 matrix = transform.toMat4();
	glm::vec3 forward = matrix[2];
	glm::vec3 front = glm::normalize(glm::vec3(forward.x, 0, forward.z));
	glm::vec3 right = matrix[0];

	if (Window::getKey(GLFW_KEY_W)) {
		transform.position -= front * (float)Time::deltaTime * speed;
	}
	if (Window::getKey(GLFW_KEY_S)) {
		transform.position += front * (float)Time::deltaTime * speed;
	}
	if (Window::getKey(GLFW_KEY_A)) {
		transform.position -= right * (float)Time::deltaTime * speed;
	}
	if (Window::getKey(GLFW_KEY_D)) {
		transform.position += right * (float)Time::deltaTime * speed;
	}
	if (Window::getKey(GLFW_KEY_SPACE)) {
		transform.position.y += (float)Time::deltaTime * speed;
	}
	if (Window::getKey(GLFW_KEY_LEFT_SHIFT)) {
		transform.position.y -= (float)Time::deltaTime * speed;
	}

	glm::vec2 cursorPos = Window::getCursorPos();

	transform.rotation.y -= (cursorPos.x - oldCursorPos.x) * sensitivity;
	transform.rotation.x -= (cursorPos.y - oldCursorPos.y) * sensitivity;

	transform.rotation.x = glm::clamp(transform.rotation.x, -80.0f, 80.0f);

	oldCursorPos = cursorPos;
}