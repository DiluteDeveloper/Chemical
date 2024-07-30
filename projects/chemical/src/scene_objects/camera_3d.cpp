#include <pch.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "camera_3d.h"

namespace Chemical {

	namespace Scene {
		Camera3D::Camera3D(const Core::GLFWWrapper& wrapper, Core::EventDispatcher& dispatcher) :
			m_wrapper(wrapper), object(this, "Camera3D") {
			dispatcher.Subscribe(Core::InputEvent::descriptor, std::bind(&Camera3D::OnInput, this, std::placeholders::_1));
		}

		glm::mat4 Camera3D::UpdateMovement() {

			Object3D::Transform& transform = object_3d.transform;
			glm::fmat4 matrix = transform.TransformToMat4();

			if (m_enabled) {


				glm::fvec3 right = matrix[0];
				glm::fvec3 up = matrix[1];
				glm::fvec3 forward = matrix[2];
				glm::fvec3 front = glm::normalize(glm::fvec3(forward.x, 0, forward.z));

				GLFWwindow* window = m_wrapper.GetGLFWWindow();
				if (glfwGetKey(window, GLFW_KEY_W)) {
					transform.position += front * m_cam_speed;
				}
				if (glfwGetKey(window, GLFW_KEY_S)) {
					transform.position -= front * m_cam_speed;
				}
				if (glfwGetKey(window, GLFW_KEY_A)) {
					transform.position -= right * m_cam_speed;
				}
				if (glfwGetKey(window, GLFW_KEY_D)) {
					transform.position += right * m_cam_speed;
				}
				if (glfwGetKey(window, GLFW_KEY_SPACE)) {
					transform.position.y += m_cam_speed;
				}
				if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
					transform.position.y -= m_cam_speed;
				}

				double x, y;
				glfwGetCursorPos(window, &x, &y);

				transform.rotation.y += static_cast<float>(x - m_oldx) * m_sensitivity;
				transform.rotation.x += static_cast<float>(y - m_oldy) * m_sensitivity;

				transform.rotation.x = glm::clamp(transform.rotation.x, -85.0f, 85.0f);

				m_oldx = x;
				m_oldy = y;
			}

			return matrix;
		}

		float Camera3D::GetCameraSpeed() {
			return m_sensitivity;
		}
		void Camera3D::SetCameraSpeed(float speed) {
			m_cam_speed = speed;
		}
		float Camera3D::GetCameraSensitivity() {
			return m_sensitivity;
		}
		void Camera3D::SetCameraSensitivity(float sensitivity) {
			m_sensitivity = sensitivity;
		}

		void Camera3D::OnInput(const Core::Event& event) {
			const Core::InputEvent& event_actual = static_cast<const Core::InputEvent&>(event);

			if (event_actual.key == GLFW_KEY_ESCAPE && event_actual.action == GLFW_PRESS) {

				if (!m_enabled)
					glfwGetCursorPos(event_actual.window, &m_oldx, &m_oldy);
				m_enabled = !m_enabled;
			}
		}
	}


}