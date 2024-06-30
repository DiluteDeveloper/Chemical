#include <pch.h>

#include "window.h"

namespace Chemical {
	namespace Core {

#ifdef CHEMICAL_DEBUG

		// prevents message duplication
		int64_t prev_message_id = -1;

		// OpenGL debug callback
		void APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
		{

			if ((GLuint)prev_message_id == id)
				return;
			prev_message_id = id;
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
						LOGGER_CONSOLE_CUSTOM_MESSAGE("{}, {}, {}: {}", src_str, type_str, (int)id, (char*)message);
						break;
					case GL_DEBUG_SEVERITY_LOW:
						LOGGER_CONSOLE_CUSTOM_WARNING("{}, {}, {}: {}", src_str, type_str, (int)id, (char*)message);
						break;
					case GL_DEBUG_SEVERITY_MEDIUM:
						LOGGER_CONSOLE_CUSTOM_WARNING("{}, {}, {}: {}", src_str, type_str, (int)id, (char*)message);
						break;
					case GL_DEBUG_SEVERITY_HIGH:
						LOGGER_CONSOLE_CUSTOM_ERROR("{}, {}, {}: {}", src_str, type_str, (int)id, (char*)message);
						throw std::exception();
						break;
					default: break;
					}

		}
#endif

		GLFWwindow* Window::glfw_window = nullptr;
		const std::string Window::glsl_version = "#version 460";
		const glm::ivec2 Window::window_size = glm::ivec2(1280, 720);
		const std::string Window::window_name = "Chemical";

		void Window::InitializeWindow() {
			// GLFW INITIALIZATION ----------------------------

			if (!glfwInit()) {
#ifdef CHEMICAL_DEBUG
				LOGGER_CONSOLE_ERROR("GLFW initialization failed.");
#endif
				throw std::exception();
			}
#ifdef CHEMICAL_DEBUG
			else
				LOGGER_CONSOLE_MESSAGE("GLFW initialized.");
#endif

			// GLFW INITIALIZATION ----------------------------

			// GLFW WINDOW HINTS ----------------------------------

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


			// GLFW WINDOW HINTS ----------------------------------

			// GLFW WINDOW SETUP -------------------------------------------------

			glfw_window = glfwCreateWindow(window_size.x, window_size.y, window_name.c_str(), NULL, NULL);

			if (!glfw_window) {
#ifdef CHEMICAL_DEBUG
				LOGGER_CONSOLE_ERROR("GLFW window creation failed.");
#endif
				throw std::exception();
			}
#ifdef CHEMICAL_DEBUG
			else
				LOGGER_CONSOLE_MESSAGE("GLFW window created.");
#endif

			// GLFW WINDOW SETUP -------------------------------------------------

			// GLFW PREFERENCES ----------------------------------

			glfwSwapInterval(1);

			// GLFW PREFERENCES ----------------------------------

			// GLAD SETUP --------------------------------------------

			glfwMakeContextCurrent(glfw_window);

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

			glClearColor(1.0f, 0.2f, 0.3f, 1.0f);

			// GLAD SETUP --------------------------------------------

			// GLAD PREFERENCES -----------------------------------------

			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glEnable(GL_DEPTH_TEST);

#ifdef CHEMICAL_DEBUG
			glDebugMessageCallback(&MessageCallback, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

			// GLAD PREFERENCES -----------------------------------------
		}

		bool Window::WindowShouldClose() {
			return glfwWindowShouldClose(glfw_window);
		}
		void Window::SwapBuffers() {

			glfwSwapBuffers(glfw_window);
		}
	}
}