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

		class GLFWWrapper {
		public:

			GLFWWrapper(std::shared_ptr<EventDispatcher> dispatcher, unsigned int window_size_x = 720, unsigned int window_size_y = 480);

			unsigned int GetWindowSizeX() const;
			unsigned int GetWindowSizeY() const;
			const std::string& GetGLSLVersion() const;
			GLFWwindow* GetGLFWWindow() const;

		private:
			static void _WindowCloseCallback(GLFWwindow* window);
			static void _KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

			static std::shared_ptr<EventDispatcher> _dispatcher;

			unsigned int _window_size_x = 0;
			unsigned int _window_size_y = 0;

			std::string _glsl_version = "#version 460";
			GLFWwindow* _window = nullptr;
		};
	}

}
