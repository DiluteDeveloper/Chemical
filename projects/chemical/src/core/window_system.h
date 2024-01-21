#pragma once

struct GLFWwindow;

namespace Chemical {
	namespace Core {

		// pretty much just a GLFW wrapper atleast for windowing code
		class WindowSystem {
		public:
			
			// called by engine only
			WindowSystem(int width = 1280, int height = 720, const std::string_view& title = "Chemical");

			bool shouldWindowClose();

			// only public to be sent for use to other engine components
			GLFWwindow* window;

		};
	}
}
