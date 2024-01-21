#pragma once

struct GLFWwindow;


namespace Chemical {
	namespace Core {

		class Input {
		public:

			// Called by engine only
			// Must be constructed after the GLFW window.
			Input(GLFWwindow* window);

			// return value for GetKeyState
			enum class KeyState {
				RELEASE = 0,
				PRESS = 1
			};

			// key should be a enum at some point
			KeyState GetKeyState(int key);


			// Called by engine only
			void PollEvents();

		private:

			// Not gonna abstract GLFW out to restrict access. GLFW windows handles many things, 
			// input is one of them, deal with it
			GLFWwindow* window = nullptr;
		};
	}
}
