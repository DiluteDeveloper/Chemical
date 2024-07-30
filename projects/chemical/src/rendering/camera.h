#pragma once

#include "util/transform.h"
#include "core/event_dispatcher.h"

namespace Chemical {

	namespace Core {
		class GLFWWrapper;
	}
	class Camera {
	public:
		Camera(const Core::GLFWWrapper& wrapper, Core::EventDispatcher& dispatcher);
		glm::mat4 UpdateMovement();
	private:
		const Core::GLFWWrapper& m_wrapper;
		bool m_enabled = true;

		void OnInput(const Core::Event& event);

		Transform m_transform;

		float m_cam_speed = 0.05f;
		float m_sensitivity = 0.08f;

		double m_oldx = 0, m_oldy = 0;
	};
}