#include <pch.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "camera_3d.h"
#include "scene.h"

namespace Chemical {

	void Camera3D::OnSceneLoad() {
		GetScene().GetEventDispatcher().Subscribe(Core::InputEvent::descriptor, std::bind(&Camera3D::OnInput, this, std::placeholders::_1));
	}

	void Camera3D::OnSceneUpdate() {

		m_view = transform.TransformToMat4();

		if (enabled) {

			GLFWwindow* window = GetScene().GetGLFWWrapper().GetGLFWWindow();

			glm::fvec3 right = m_view[0];
			glm::fvec3 up = m_view[1];
			glm::fvec3 forward = m_view[2];
			glm::fvec3 front = glm::normalize(glm::fvec3(forward.x, 0, forward.z));

			if (window == nullptr)
				return;
			if (glfwGetKey(window, GLFW_KEY_W)) {
				transform.position += front * m_camSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_S)) {
				transform.position -= front * m_camSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_A)) {
				transform.position -= right * m_camSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_D)) {
				transform.position += right * m_camSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_SPACE)) {
				transform.position.y += m_camSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
				transform.position.y -= m_camSpeed;
			}

			double x = 0, y = 0;
			glfwGetCursorPos(window, &x, &y);

			transform.rotation.y += static_cast<float>(x - m_oldX) * m_sensitivity;
			transform.rotation.x += static_cast<float>(y - m_oldY) * m_sensitivity;

			transform.rotation.x = glm::clamp(transform.rotation.x, -85.0f, 85.0f);

			m_oldX = x;
			m_oldY = y;

		}
	}

	float Camera3D::GetCameraSpeed() const {
		return m_sensitivity;
	}
	void Camera3D::SetCameraSpeed(float speed) {
		m_camSpeed = speed;
	}
	float Camera3D::GetCameraSensitivity() const {
		return m_sensitivity;
	}
	void Camera3D::SetCameraSensitivity(float sensitivity) {
		m_sensitivity = sensitivity;
	}

	void Camera3D::OnInput(const Core::Event& event) {
		const Core::InputEvent& event_actual = static_cast<const Core::InputEvent&>(event);

		if (event_actual.key == GLFW_KEY_ESCAPE && event_actual.action == GLFW_PRESS) {

			if (!enabled)
				glfwGetCursorPos(event_actual.window, &m_oldX, &m_oldY);
			enabled = !enabled;
		}
	}


}