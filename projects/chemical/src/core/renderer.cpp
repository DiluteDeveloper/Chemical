#include "pch.h"

#include "renderer.h"

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

namespace Chemical {
	namespace Core {

		Renderer::Renderer(GLFWwindow* window, glm::fvec3 clearColour) : window(window) {
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
#ifdef CHEMICAL_DEBUG
				LOGGER_CONSOLE_ERROR("gladLoadGL failed.");
#endif
				throw std::exception();
			}
#ifdef CHEMICAL_DEBUG
			else
				LOGGER_CONSOLE_MESSAGE("gladLoadGL succeeded.");
#endif

			glClearColor(clearColour.r, clearColour.y, clearColour.b, 1.0f);

		}

		void Renderer::Update() {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// rendering goes here

			glfwSwapBuffers(window);
		}
	}
}
