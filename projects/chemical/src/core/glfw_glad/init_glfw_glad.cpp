#include <pch.h>

#include "util/console_logger.h"

namespace Chemical {

#ifdef CHEMICAL_DEBUG

	// prevents message duplication
	int64_t prevMessageID = -1;

	// OpenGL debug callback
	void APIENTRY message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
	{

		if ((GLuint)prevMessageID == id)
			return;
		prevMessageID = id;
		auto const src_str = [source]() {
			switch (source)
			{
			case GL_DEBUG_SOURCE_API: return "API";
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
			case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
			case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
			case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
			case GL_DEBUG_SOURCE_OTHER: return "OTHER";
			}
			return "SOURCE";
			}();


			auto const type_str = [type]() {
				switch (type)
				{
				case GL_DEBUG_TYPE_ERROR: return "ERROR";
				case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
				case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
				case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
				case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
				case GL_DEBUG_TYPE_MARKER: return "MARKER";
				case GL_DEBUG_TYPE_OTHER: return "OTHER";
				default: break;
				}
				return "TYPE";
				}();

				switch (severity) {
				case GL_DEBUG_SEVERITY_NOTIFICATION:
					CONSOLE_CUSTOM_PRINT(Severity::_DEFAULT, "OpenGL Debug Notification: {}, {}, {}: {}", src_str, type_str, (int)id, (char*)message);
					break;
				case GL_DEBUG_SEVERITY_LOW:
					CONSOLE_CUSTOM_PRINT(Severity::_WARNING, "OpenGL Debug Low Warning: {}, {}, {}: {}", src_str, type_str, (int)id, (char*)message);
					break;
				case GL_DEBUG_SEVERITY_MEDIUM:
					CONSOLE_CUSTOM_PRINT(Severity::_WARNING, "OpenGL Debug Medium Warning: {}, {}, {}: {}", src_str, type_str, (int)id, (char*)message);
					break;
				case GL_DEBUG_SEVERITY_HIGH:
					CONSOLE_CUSTOM_PRINT(Severity::_ERROR, "OpenGL Debug Error: {}, {}, {}: {}", src_str, type_str, (int)id, (char*)message);
					throw std::exception();
					break;
				default: break;
				}

	}
#endif

	GLFWwindow* window = nullptr;
	unsigned int window_size_x = 0;
	unsigned int window_size_y = 0;
	bool InitializeGLFWGLAD(unsigned int f_window_size_x, unsigned int f_window_size_y) {
		window_size_x = f_window_size_x;
		window_size_y = f_window_size_y;

		// GLFW INITIALIZATION ----------------------------

		if (!glfwInit()) {
			CONSOLE_PRINT(Severity::_ERROR, "GLFW initialization failed.");
			return false;
		} else
			CONSOLE_PRINT(Severity::_SUCCESS, "GLFW initialization successful.");


		// GLFW INITIALIZATION ----------------------------

		// GLFW WINDOW HINTS ----------------------------------

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


		// GLFW WINDOW HINTS ----------------------------------

		// GLFW WINDOW SETUP -------------------------------------------------

		window = glfwCreateWindow(window_size_x, window_size_y, "Chemical", NULL, NULL);

		if (!window) {
			CONSOLE_PRINT(Severity::_ERROR, "Creating GLFW window failed.");
			return false;
		} else
			CONSOLE_PRINT(Severity::_SUCCESS, "GLFW window successfully created.");


		// GLFW WINDOW SETUP -------------------------------------------------

		// GLFW PREFERENCES ----------------------------------

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSwapInterval(1);

		// GLFW PREFERENCES ----------------------------------

		// GLAD SETUP --------------------------------------------

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			CONSOLE_PRINT(Severity::_ERROR, "GLAD functions failed to load.");
			return false;
		} else 
			CONSOLE_PRINT(Severity::_SUCCESS, "GLAD functions successfully loaded.");


		glClearColor(1.0f, 0.2f, 0.3f, 1.0f);

		// GLAD SETUP --------------------------------------------

		// GLAD PREFERENCES -----------------------------------------

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

		glEnable(GL_DEPTH_TEST);


		DEBUG_CALL(glDebugMessageCallback(&message_callback, nullptr));
		DEBUG_CALL(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE));

		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		//glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

		return true;

	}

	unsigned int GetWindowSizeX() {
		return window_size_x;
	}
	unsigned int GetWindowSizeY() {
		return window_size_y;
	}
}