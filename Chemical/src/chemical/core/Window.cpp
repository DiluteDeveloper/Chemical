#include "Window.h"

#include <iostream>

#include <glfw/glfw3.h>
#include <glad/glad.h>

#include <vendor/IMGUI/imgui_impl_glfw.h>
#include <vendor/IMGUI/imgui_impl_opengl3.h>
#include <vendor/IMGUI/imgui.h>

namespace Chemical {

	namespace Core {

		void APIENTRY DebugCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam) {

			if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
				return;
			std::cout << "[OPENGL]" << std::endl;
			std::cout << "[ERROR:" << id << "]" << std::endl;

			switch (source) {
			case GL_DEBUG_SOURCE_API:
				std::cout << "[SOURCE:OPENGL ERROR GL_DEBUG_SOURCE_API]" << std::endl;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
				std::cout << "[SOURCE:GL_DEBUG_SOURCE_WINDOW_SYSTEM]" << std::endl;
				break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER:
				std::cout << "[SOURCE:GL_DEBUG_SOURCE_SHADER_COMPILER]" << std::endl;
				break;
			case GL_DEBUG_SOURCE_THIRD_PARTY:
				std::cout << "[SOURCE:GL_DEBUG_SOURCE_THIRD_PARTY]" << std::endl;
				break;
			case GL_DEBUG_SOURCE_APPLICATION:
				std::cout << "[SOURCE:GL_DEBUG_SOURCE_APPLICATION]" << std::endl;
				break;
			case GL_DEBUG_SOURCE_OTHER:
				std::cout << "[SOURCE:GL_DEBUG_SOURCE_OTHER]" << std::endl;
				break;
			}

			switch (type) {
			case GL_DEBUG_TYPE_ERROR:
				std::cout << "[TYPE:GL_DEBUG_TYPE_ERROR" << std::endl;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				std::cout << "[TYPE:GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR]" << std::endl;
				break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				std::cout << "[TYPE:GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR]" << std::endl;
				break;
			case GL_DEBUG_TYPE_PORTABILITY:
				std::cout << "[TYPE:GL_DEBUG_SOURCE_THIRD_PARTY]" << std::endl;
				break;
			case GL_DEBUG_TYPE_PERFORMANCE:
				std::cout << "[TYPE:GL_DEBUG_SOURCE_APPLICATION]" << std::endl;
				break;
			case GL_DEBUG_TYPE_MARKER:
				std::cout << "[TYPE:GL_DEBUG_SOURCE_OTHER]" << std::endl;
				break;
			case GL_DEBUG_TYPE_PUSH_GROUP:
				std::cout << "[TYPE:GL_DEBUG_TYPE_PUSH_GROUP]" << std::endl;
				break;
			case GL_DEBUG_TYPE_POP_GROUP:
				std::cout << "[TYPE:GL_DEBUG_TYPE_POP_GROUP]" << std::endl;
				break;
			case GL_DEBUG_TYPE_OTHER:
				std::cout << "[TYPE:GL_DEBUG_TYPE_OTHER]" << std::endl;
				break;
			}

			switch (severity) {
			case GL_DEBUG_SEVERITY_HIGH:
				std::cout << "[SEVERITY:GL_DEBUG_SEVERITY_HIGH]" << std::endl;
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				std::cout << "[SEVERITY:GL_DEBUG_SEVERITY_MEDIUM]" << std::endl;
				break;
			case GL_DEBUG_SEVERITY_LOW:
				std::cout << "[SEVERITY:GL_DEBUG_SEVERITY_LOW]" << std::endl;
				break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				std::cout << "[SEVERITY:GL_DEBUG_SEVERITY_NOTIFICATION]" << std::endl;
				break;
			}

			std::cout << "[MESSAGE:" << message << "]" << std::endl;

		}

		void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
			glViewport(0, 0, width, height);
			//WindowResizeDispatcher::invoke(width, height);
		}

		void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
			if (glfwGetKey(window, GLFW_KEY_ESCAPE))
				glfwSetWindowShouldClose(window, true);

			//KeyPressDispatcher::invoke(key, scancode, action, mods);

		}

		Window::Window(AccessKey<Application>) {
			std::cout << "Window constructor called." << std::endl;

			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			glfwWindow = glfwCreateWindow(1280, 720, "LearnOpenGL", NULL, NULL);
			if (glfwWindow == NULL) {
				std::cout << "Failed to create GLFW window" << std::endl;
				glfwTerminate();
				return;
			}
			glfwMakeContextCurrent(glfwWindow);

			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
				std::cout << "Failed to initialize GLAD" << std::endl;
				return;
			}

			glfwSetFramebufferSizeCallback(glfwWindow, FramebufferSizeCallback);
			glfwSetKeyCallback(glfwWindow, KeyCallback);
			glDebugMessageCallback(&DebugCallback, nullptr);

			glfwSwapInterval(0);

			if (glfwRawMouseMotionSupported())
				glfwSetInputMode(glfwWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);
			glCullFace(GL_BACK);

			glEnable(GL_DEPTH_TEST);

			glEnable(GL_DEBUG_OUTPUT);

			glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			ImGui::StyleColorsDark();
			ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
			ImGui_ImplOpenGL3_Init("#version 460");

		}

		Window::~Window() {
			std::cout << "Window destructor called." << std::endl;
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();

			glfwTerminate();
		}

		void Window::SetInputMode(int mode, int value) const {
			glfwSetInputMode(glfwWindow, mode, value);
		}

		glm::ivec2 Window::GetSize() const{
			int x, y;
			glfwGetWindowSize(glfwWindow, &x, &y);
			return glm::ivec2(x, y);
		}

		void Window::SwapBuffers() const {
			glfwSwapBuffers(glfwWindow);
		}

		bool Window::Close() const {
			return glfwWindowShouldClose(glfwWindow);
		}

		bool Window::GetKey(int key) const {
			return glfwGetKey(glfwWindow, key);
		}
		glm::ivec2 Window::GetCursorPos() const {
			double x, y;
			glfwGetCursorPos(glfwWindow, &x, &y);
			return glm::ivec2(x, y);
		}

		void Window::PollEvents() const {
			glfwPollEvents();
		}
		double Window::GetTime() const {
			return glfwGetTime();
		}
	}

}
