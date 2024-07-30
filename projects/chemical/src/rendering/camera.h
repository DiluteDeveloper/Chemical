#pragma once

#include "util/transform.h"

namespace Chemical {

	namespace Core {
		class GLFWWrapper;
	}
	class Camera {
	public:
		Camera(const Core::GLFWWrapper& wrapper);
		glm::mat4 UpdateMovement();

		bool GetEnabled() const;
		void SetEnabled(bool enabled);
	private:
		const Core::GLFWWrapper& m_wrapper;
		bool m_enabled = true;

		Transform m_transform;

		float m_cam_speed = 0.05f;
		float m_sensitivity = 0.08f;

		double m_oldx = 0, m_oldy = 0;
	};
}