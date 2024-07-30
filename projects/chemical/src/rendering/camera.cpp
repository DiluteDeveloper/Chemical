#include <pch.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "core/glfw_wrapper.h"

namespace Chemical {

	Camera::Camera(const Core::GLFWWrapper& wrapper) :
		m_wrapper(wrapper) {}

	glm::mat4 Camera::UpdateMovement() {

		glm::fmat4 matrix = m_transform.TransformToMat4();

		if (m_enabled) {


			glm::fvec3 right = matrix[0];
			glm::fvec3 up = matrix[1];
			glm::fvec3 forward = matrix[2];
			glm::fvec3 front = glm::normalize(glm::fvec3(forward.x, 0, forward.z));

			GLFWwindow* window = m_wrapper.GetGLFWWindow();
			if (glfwGetKey(window, GLFW_KEY_W)) {
				m_transform.position += front * m_cam_speed;
			}
			if (glfwGetKey(window, GLFW_KEY_S)) {
				m_transform.position -= front * m_cam_speed;
			}
			if (glfwGetKey(window, GLFW_KEY_A)) {
				m_transform.position -= right * m_cam_speed;
			}
			if (glfwGetKey(window, GLFW_KEY_D)) {
				m_transform.position += right * m_cam_speed;
			}
			if (glfwGetKey(window, GLFW_KEY_SPACE)) {
				m_transform.position.y += m_cam_speed;
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
				m_transform.position.y -= m_cam_speed;
			}

			double x, y;
			glfwGetCursorPos(window, &x, &y);

			m_transform.rotation.y += static_cast<float>(x - m_oldx) * m_sensitivity;
			m_transform.rotation.x += static_cast<float>(y - m_oldy) * m_sensitivity;

			m_transform.rotation.x = glm::clamp(m_transform.rotation.x, -85.0f, 85.0f);

			m_oldx = x;
			m_oldy = y;
		}

		return matrix;
	}

	bool Camera::GetEnabled() const {
		return m_enabled;
	}
	void Camera::SetEnabled(bool enabled) {
		m_enabled = enabled;
		if (enabled) {
			glfwGetCursorPos(m_wrapper.GetGLFWWindow(), &m_oldx, &m_oldy);
		}
	}

}