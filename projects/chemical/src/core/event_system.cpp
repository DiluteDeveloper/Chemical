#include <pch.h>

#include "event_system.h"

namespace Chemical {

	void EventSystemGLFWCallbackReceiver::SetEventSystem(EventSystem* f_event_system) {
		event_system = f_event_system;
	}

	void EventSystemGLFWCallbackReceiver::WindowCloseCallback(GLFWwindow* window) {
		event_system->dispatch_window_close_event(window);
	}

	void EventSystemGLFWCallbackReceiver::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		CHEMICAL_CUSTOM_PRINT(Severity::_SUCCESS, "KEY: {}, ACTION: {}", key, action);

		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			int mode = glfwGetInputMode(window, GLFW_CURSOR);
			if(mode == GLFW_CURSOR_DISABLED)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			else
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}

	EventSystem::EventSystem(GLFWWrapper* wrapper) {
		wrapper->SetStaticCallbackReceiver(&glfw_callback_receiver);

		glfw_callback_receiver.SetEventSystem(this);
	}

	void EventSystem::dispatch_window_close_event(GLFWwindow* window) {
		for (auto f : window_close_event_subscribers) {
			f(window);
		}
	}
	void EventSystem::subscribe_window_close_event(const std::function<void(GLFWwindow*)>& subscriber) {
		window_close_event_subscribers.emplace_back(subscriber);
	}
}