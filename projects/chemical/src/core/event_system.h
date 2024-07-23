#pragma once

#include <functional>

#include "glfw_glad/init_glfw_glad.h"

namespace Chemical {

	class EventSystem;
	class EventSystemGLFWCallbackReceiver : public GLFWCallbackReceiver {
	public:
		void SetEventSystem(EventSystem* f_event_system);

		void WindowCloseCallback(GLFWwindow* window) override;
		void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) override;
	private:
		EventSystem* event_system;
	};

	class EventSystem {
	public:

		EventSystem(GLFWWrapper* wrapper);

		enum class Event {
			WindowCloseEvent
		};

		void subscribe_window_close_event(const std::function<void(GLFWwindow*)>& subscriber);
		void dispatch_window_close_event(GLFWwindow* window);

	private:
		EventSystemGLFWCallbackReceiver glfw_callback_receiver;

		std::vector<std::function<void(GLFWwindow*)>> window_close_event_subscribers;
	};

}
