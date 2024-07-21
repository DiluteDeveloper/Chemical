#include <pch.h>

#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"
#include "core/glfw_glad/init_glfw_glad.h"
#include <util/transform.h>

namespace Chemical {
	namespace Camera {

		Transform transform;
		glm::mat4 view = glm::mat4(1.0f);
		float camSpeed = 0.05f;
		float sensitivity = 0.08f;
		// movement controls
		double oldx, oldy;
		void UpdateCamera() {
			glm::fmat4 matrix = transform.TransformToMat4();
			glm::fvec3 right = matrix[0];
			glm::fvec3 up = matrix[1];
			glm::fvec3 forward = matrix[2];
			glm::fvec3 front = glm::normalize(glm::fvec3(forward.x, 0, forward.z));

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

			view = transform.TransformToMat4();
		}
	}
}