#include <pch.h>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "glfw_wrapper.h"

namespace Chemical {

	namespace Core {

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
					spdlog::info("OpenGL Debug Notification: {0}, {1}, {2}: {3}", src_str, type_str, (int)id, (char*)message);
					break;
				case GL_DEBUG_SEVERITY_LOW:
					spdlog::warn("OpenGL Debug Low Warning: {0}, {1}, {2}: {3}", src_str, type_str, (int)id, (char*)message);
					break;
				case GL_DEBUG_SEVERITY_MEDIUM:
					spdlog::error("OpenGL Debug Medium Warning: {0}, {1}, {2}: {3}", src_str, type_str, (int)id, (char*)message);
					break;
				case GL_DEBUG_SEVERITY_HIGH:
					spdlog::critical("OpenGL Debug Error: {0}, {1}, {2}: {3}", src_str, type_str, (int)id, (char*)message);
					throw std::exception();
					break;
				default: break;
				}

		}
	EventDispatcher* GLFWWrapper::m_dispatcher = nullptr;

	void GLFWWrapper::WindowCloseCallback(GLFWwindow* window) {
		m_dispatcher->Post(WindowCloseEvent(window));
	}
	void GLFWWrapper::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		m_dispatcher->Post(InputEvent(window, key, scancode, action, mods));
	}
	GLFWWrapper::GLFWWrapper(EventDispatcher* dispatcher, unsigned int window_size_x, unsigned int window_size_y) :
		m_window_size_x(window_size_x), m_window_size_y(window_size_y) {

		m_dispatcher = dispatcher;

		// GLFW INITIALIZATION ----------------------------

		if (!glfwInit()) {
			spdlog::critical("GLFW initialization failed.");
			throw std::exception();
		}
		spdlog::info("GLFW initialization successful.");


		// GLFW INITIALIZATION ----------------------------

		// GLFW WINDOW HINTS ----------------------------------

		// not tied to glsl_version, if this is changed, need to change that
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


		// GLFW WINDOW HINTS ----------------------------------

		// GLFW WINDOW SETUP -------------------------------------------------

		m_window = glfwCreateWindow(window_size_x, window_size_y, "Chemical", NULL, NULL);

		if (!m_window) {
			spdlog::critical("GLFW window creation failed.");
		}
		spdlog::info("GLFW window creation successful.");


		// GLFW WINDOW SETUP -------------------------------------------------

		// GLFW PREFERENCES ----------------------------------

		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSwapInterval(1);

		// GLFW PREFERENCES ----------------------------------

		// GLAD SETUP --------------------------------------------

		glfwMakeContextCurrent(m_window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			spdlog::critical("GLAD functions failed to load.");
			throw std::exception();
		}
		spdlog::info("GLAD functions loaded successfully.");


		glClearColor(1.0f, 0.2f, 0.3f, 1.0f);

		// GLAD SETUP --------------------------------------------

		// GLAD PREFERENCES -----------------------------------------

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

		glEnable(GL_DEPTH_TEST);


		glDebugMessageCallback(&message_callback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		//glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

		glfwSetWindowCloseCallback(m_window, WindowCloseCallback);
		glfwSetKeyCallback(m_window, KeyCallback);


		glfwSwapInterval(1);

		spdlog::info("GLFWWrapper creation successful.");

		}

		unsigned int GLFWWrapper::GetWindowSizeX() const {
			return m_window_size_x;
		}
		unsigned int GLFWWrapper::GetWindowSizeY() const {
			return m_window_size_y;
		}

		const std::string& GLFWWrapper::GetGLSLVersion() const {
			return m_glsl_version;
		}
		GLFWwindow* GLFWWrapper::GetGLFWWindow() const {
			return m_window;
		}

		GLFWWrapper::~GLFWWrapper() {
			glfwDestroyWindow(m_window);
			glfwTerminate();
		}
	}


}