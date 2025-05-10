#include "pch.h"

#include "chemical/graphics/misc.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace Chemical {
	namespace Graphics {

		void SetBackgroundColour(RGBColour colour) {
			glClearColor(colour.r / 255.0f, colour.g / 255.0f, colour.b / 255.0f, 1.0f);
		}

		void ClearColourBuffer() {
			glClear(GL_COLOR_BUFFER_BIT);
		}
		void SwapBuffers(Window window) {
			glfwSwapBuffers(window);
		}
	}
}