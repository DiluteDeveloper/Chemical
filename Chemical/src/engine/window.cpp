#include "window.h"
#include "events.h"

#include <iostream>

#include <GLFW/glfw3.h>
#include <vendor/GLAD/glad.h>

#include <vendor/IMGUI/imgui_impl_glfw.h>
#include <vendor/IMGUI/imgui_impl_opengl3.h>
#include <vendor/IMGUI/imgui.h>

GLFWwindow* Window::glfwWindow = nullptr;

void APIENTRY debug_callback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam) {

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	WindowResizeDispatcher::invoke(width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);

	KeyPressDispatcher::invoke(key, scancode, action, mods);

}

void Window::init() {
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

	glfwSetFramebufferSizeCallback(glfwWindow, framebuffer_size_callback);
	glfwSetKeyCallback(glfwWindow, key_callback);
	glDebugMessageCallback(&debug_callback, nullptr);

	glfwSwapInterval(1);

	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(glfwWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_DEBUG_OUTPUT);

	glClearColor(0.3f,0.3f,0.3f, 1.0f);

}

void Window::setInputMode(int a, int b) {
	glfwSetInputMode(glfwWindow, a, b);
}

void Window::initGUI() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

glm::vec2 Window::getSize() {
	int x, y;
	glfwGetWindowSize(glfwWindow, &x, &y);
	return glm::vec2(x, y);
}

void Window::swapBuffers() {
	glfwSwapBuffers(glfwWindow);
}

bool Window::close() {
	return glfwWindowShouldClose(glfwWindow);
}

bool Window::getKey(int key) {
	return glfwGetKey(glfwWindow, key);
}
glm::vec2 Window::getCursorPos() {
	double x, y;
	glfwGetCursorPos(glfwWindow, &x, &y);
	return glm::vec2(x, y);
}