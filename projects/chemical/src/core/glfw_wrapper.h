#pragma once

#include "event_dispatcher.h"

struct GLFWwindow;

namespace Chemical {

	namespace Core {
		struct InputEvent : Event {
		public:

			InputEvent(GLFWwindow* window, int key, int scancode, int action, int mods) :
				window(window), key(key), scancode(scancode), action(action), mods(mods) {}
			InputEvent() {};

			GLFWwindow* window = nullptr;
			int key = 0;
			int scancode = 0;
			int action = 0;
			int mods = 0;

			static constexpr DescriptorType descriptor = "InputEvent";

			virtual DescriptorType GetType() const override {
				return descriptor;
			}
		};
		struct WindowCloseEvent : Event {
		public:
			WindowCloseEvent(GLFWwindow* window) :
				window(window) {}
		
			GLFWwindow* window = nullptr;

			static constexpr DescriptorType descriptor = "WindowCloseEvent";

			virtual DescriptorType GetType() const override {
				return descriptor;
			}
		};

		class GLFWWrapper {
		public:

			GLFWWrapper(EventDispatcher* dispatcher, unsigned int window_size_x = 720, unsigned int window_size_y = 480);
			~GLFWWrapper();

			unsigned int GetWindowSizeX() const;
			unsigned int GetWindowSizeY() const;
			const std::string& GetGLSLVersion() const;
			GLFWwindow* GetGLFWWindow() const;

		private:
			static void WindowCloseCallback(GLFWwindow* window);
			static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

			static EventDispatcher* m_dispatcher;

			unsigned int m_window_size_x = 0;
			unsigned int m_window_size_y = 0;

			std::string m_glsl_version = "#version 460";
			GLFWwindow* m_window = nullptr;
		};
	}

}
