#pragma once

#include "scene_objects/object_3d.h"
#include "scene_objects/object.h"
#include "core/event_dispatcher.h"
#include "core/glfw_wrapper.h"

namespace Chemical {

	namespace Scene {
		class Camera3D {
		public:
			Camera3D(const Core::GLFWWrapper& wrapper, Core::EventDispatcher& dispatcher);
			glm::mat4 UpdateMovement();

			float GetCameraSpeed();
			void SetCameraSpeed(float speed);
			float GetCameraSensitivity();
			void SetCameraSensitivity(float sensitivity);

			Object3D object_3d;
			Object object;
		private:
			const Core::GLFWWrapper& m_wrapper;
			bool m_enabled = true;

			void OnInput(const Core::Event& event);

			float m_cam_speed = 0.05f;
			float m_sensitivity = 0.08f;

			double m_oldx = 0, m_oldy = 0;
		};
	}

}