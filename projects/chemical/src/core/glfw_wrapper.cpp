#include <pch.h>
#include <GLAD/glad.h>
#include "glfw_wrapper.h"
#include "defines.h"

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
					CHEMICAL_CUSTOM_PRINT(Severity::_DEFAULT, "OpenGL Debug Notification: {}, {}, {}: {}", src_str, type_str, (int)id, (char*)message);
					break;
				case GL_DEBUG_SEVERITY_LOW:
					CHEMICAL_CUSTOM_PRINT(Severity::_WARNING, "OpenGL Debug Low Warning: {}, {}, {}: {}", src_str, type_str, (int)id, (char*)message);
					break;
				case GL_DEBUG_SEVERITY_MEDIUM:
					CHEMICAL_CUSTOM_PRINT(Severity::_WARNING, "OpenGL Debug Medium Warning: {}, {}, {}: {}", src_str, type_str, (int)id, (char*)message);
					break;
				case GL_DEBUG_SEVERITY_HIGH:
					CHEMICAL_CUSTOM_PRINT(Severity::_ERROR, "OpenGL Debug Error: {}, {}, {}: {}", src_str, type_str, (int)id, (char*)message);
					throw std::exception();
					break;
				default: break;
				}

	}
#endif

	EventDispatcher* GLFWWrapper::_dispatcher = nullptr;

	void GLFWWrapper::_WindowCloseCallback(GLFWwindow* window) {
		_dispatcher->post(WindowCloseEvent(window));
	}
	void GLFWWrapper::_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		_dispatcher->post(InputEvent(window, key, scancode, action, mods));
	}
	GLFWWrapper::GLFWWrapper(EventDispatcher* dispatcher, unsigned int window_size_x, unsigned int window_size_y) :
		_window_size_x(window_size_x), _window_size_y(window_size_y) {

		_dispatcher = dispatcher;

		// GLFW INITIALIZATION ----------------------------

		if (!glfwInit()) {
			CHEMICAL_PRINT(Severity::_ERROR, "GLFW initialization failed.");
		} else
			CHEMICAL_PRINT(Severity::_SUCCESS, "GLFW initialization successful.");


		// GLFW INITIALIZATION ----------------------------

		// GLFW WINDOW HINTS ----------------------------------

		// not tied to glsl_version, if this is changed, need to change that
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


		// GLFW WINDOW HINTS ----------------------------------

		// GLFW WINDOW SETUP -------------------------------------------------

		_window = glfwCreateWindow(window_size_x, window_size_y, "Chemical", NULL, NULL);

		if (!_window) {
			CHEMICAL_PRINT(Severity::_ERROR, "Creating GLFW window failed.");
		} else
			CHEMICAL_PRINT(Severity::_SUCCESS, "GLFW window successfully created.");


		// GLFW WINDOW SETUP -------------------------------------------------

		// GLFW PREFERENCES ----------------------------------

		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSwapInterval(1);

		// GLFW PREFERENCES ----------------------------------

		// GLAD SETUP --------------------------------------------

		glfwMakeContextCurrent(_window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			CHEMICAL_PRINT(Severity::_ERROR, "GLAD functions failed to load.");
		} else 
			CHEMICAL_PRINT(Severity::_SUCCESS, "GLAD functions successfully loaded.");


		glClearColor(1.0f, 0.2f, 0.3f, 1.0f);

		// GLAD SETUP --------------------------------------------

		// GLAD PREFERENCES -----------------------------------------

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

		glEnable(GL_DEPTH_TEST);


		CHEMICAL_DEBUG_CALL(glDebugMessageCallback(&message_callback, nullptr));
		CHEMICAL_DEBUG_CALL(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE));

		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		//glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

		glfwSetWindowCloseCallback(_window, _WindowCloseCallback);
		glfwSetKeyCallback(_window, _KeyCallback);

		glfwSwapInterval(1);

	}

	unsigned int GLFWWrapper::GetWindowSizeX() const {
		return _window_size_x;
	}
	unsigned int GLFWWrapper::GetWindowSizeY() const {
		return _window_size_y;
	}

	const std::string& GLFWWrapper::GetGLSLVersion() const {
		return _glsl_version;
	}
	GLFWwindow* GLFWWrapper::GetGLFWWindow() const {
		return _window;
	}
}