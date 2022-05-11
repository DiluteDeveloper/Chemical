#pragma once

struct GLFWwindow;

#include <vendor/glm/glm.hpp>
#include <iostream>

#include "AccessKey.h"

namespace Chemical {

	namespace Core {

		class Application;
		class Window {

			GLFWwindow* glfwWindow = nullptr;

			Window(const Window&) = delete;
			Window(Window&&) = delete;

		public:

			~Window();
			Window(AccessKey<Application>);
	

			glm::ivec2 GetSize() const;

			void SwapBuffers() const;

			void SetInputMode(int mode, int value)const ;

			bool GetKey(int key) const;
			glm::ivec2 GetCursorPos() const;

			bool Close() const;

			void PollEvents() const;

			double GetTime() const;
		};
	}

}