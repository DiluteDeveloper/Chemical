#include "WindowLayer.h"

#include "chemical/io/ConsoleStream.h"

#include <vendor/glfw/glfw3.h>
#include <vendor/glad/glad.h>

#include <vendor/IMGUI/imgui_impl_glfw.h>
#include <vendor/IMGUI/imgui_impl_opengl3.h>
#include <vendor/IMGUI/imgui.h>

namespace Chemical {

	namespace Core {

		void APIENTRY DebugCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam) {

			if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
				return;

			if(severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM)
				IO::LogError("Chemical::Core::DebugCallback({}), {}, {}, {}, {}, {}, {}",
				source, type, id, severity, length, message, userParam);
			else
				IO::LogWarning("Chemical::Core::DebugCallback({}), {}, {}, {}, {}, {}, {}",
					source, type, id, severity, length, message, userParam);

		}

		void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
			glViewport(0, 0, width, height);
		}

		void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
			if (glfwGetKey(window, GLFW_KEY_ESCAPE))
				glfwSetWindowShouldClose(window, true);

			//KeyPressDispatcher::invoke(key, scancode, action, mods);

		}

		WindowLayer::WindowLayer(const WindowSettings& settings) {

			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			if(settings.fullscreen)
				glfwWindow = glfwCreateWindow(settings.resolution.x, settings.resolution.y, settings.title.c_str(), glfwGetPrimaryMonitor(), NULL);
			else 
				glfwWindow = glfwCreateWindow(settings.resolution.x, settings.resolution.y, settings.title.c_str(), NULL, NULL);
			if (glfwWindow == NULL) {
				IO::LogError("Chemical::Core::WindowLayer::WindowLayer({}) Failed to create glfw window.", settings);
				throw std::exception();
			}
			glfwMakeContextCurrent(glfwWindow);

			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
				IO::LogError("Chemical::Core::WindowLayer::WindowLayer({}) Failed to load GLAD.", settings);
				throw std::exception();
			}

			glfwSetFramebufferSizeCallback(glfwWindow, FramebufferSizeCallback);
			glfwSetKeyCallback(glfwWindow, KeyCallback);
			glDebugMessageCallback(&DebugCallback, nullptr);

			glfwSwapInterval(settings.vSync);

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

		WindowLayer::~WindowLayer() {
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();

			glfwTerminate();
		}

		double WindowLayer::GetTime() const {
			return glfwGetTime();
		}
		bool WindowLayer::ShouldClose() const {
			return glfwWindowShouldClose(glfwWindow);
		}

		void WindowLayer::PollEvents() const {
			glfwPollEvents();
		}
		void WindowLayer::SwapBuffers() const {
			glfwSwapBuffers(glfwWindow);
		}
	}

}
