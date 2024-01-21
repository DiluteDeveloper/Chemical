#include "pch.h"

#include "input.h"

#include <GLFW/glfw3.h>

namespace Chemical {
	namespace Core {

		void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			// event system functionality here
		}

		Input::Input(GLFWwindow* window) : window(window) {
			glfwSetKeyCallback(window, key_callback);

		}

		Input::KeyState Input::GetKeyState(int key) {
			return static_cast<Input::KeyState>(glfwGetKey(window, key));
		}

		void Input::PollEvents() {
			glfwPollEvents();
		}
	}
}