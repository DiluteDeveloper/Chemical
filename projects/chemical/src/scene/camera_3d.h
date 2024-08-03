#pragma once

#include "object.h"
#include "modules/transform.h"
#include "core/event_dispatcher.h"

namespace Chemical {

	class Camera3D : public Object {
	public:

		void OnSceneLoad() override;
		void OnSceneUpdate() override;

		float GetCameraSpeed() const;
		void SetCameraSpeed(float speed);
		float GetCameraSensitivity() const;
		void SetCameraSensitivity(float sensitivity);

		const glm::mat4& GetView() const {
			return m_view;
		}

		Transform transform;

	private:

		void OnInput(const Core::Event& event);

		glm::mat4 m_view = glm::mat4(1.0f);

		float m_camSpeed = 0.05f;
		float m_sensitivity = 0.08f;

		double m_oldX = 0, m_oldY = 0;
	};

}