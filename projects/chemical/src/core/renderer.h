#pragma once

struct GLFWwindow;

namespace Chemical {
	namespace Core {

		class Renderer {
		public:

			// called by engine only
			Renderer(GLFWwindow* window, glm::fvec3 clearColour = glm::fvec3(1.0f));

			// called by engine only
			void Update();

		private:
			GLFWwindow* window;
		};
	}
}
