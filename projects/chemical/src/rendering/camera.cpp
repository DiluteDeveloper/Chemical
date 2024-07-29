#include <pch.h>
#include <glm/gtc/matrix_transform.hpp>
#include "util/transform.h"
#include "camera.h"
#include "core/glfw_wrapper.h"


namespace Chemical {

	glm::mat4 Camera::UpdateMovement(const GLFWWrapper* wrapper) {
		glm::fmat4 matrix = transform.TransformToMat4();
		glm::fvec3 right = matrix[0];
		glm::fvec3 up = matrix[1];
		glm::fvec3 forward = matrix[2];
		glm::fvec3 front = glm::normalize(glm::fvec3(forward.x, 0, forward.z));

		GLFWwindow* window = wrapper->GetGLFWWindow();
		if (glfwGetKey(window, GLFW_KEY_W)) {
			transform.position += front * camSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_S)) {
			transform.position -= front * camSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_A)) {
			transform.position -= right * camSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_D)) {
			transform.position += right * camSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE)) {
			transform.position.y += camSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
			transform.position.y -= camSpeed;
		}

		double x, y;
		glfwGetCursorPos(window, &x, &y);

		transform.rotation.y += static_cast<float>(x - oldx) * sensitivity;
		transform.rotation.x += static_cast<float>(y - oldy) * sensitivity;

		transform.rotation.x = glm::clamp(transform.rotation.x, -85.0f, 85.0f);

		oldx = x;
		oldy = y;

		return matrix;
	}

}