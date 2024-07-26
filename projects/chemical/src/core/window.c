#include "window.h"

#include <stdio.h>

extern void APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param);

GLFWwindow* CMCL_CreateWindow(int width, int height, const char* title) {

	// GLFW INITIALIZATION ----------------------------

	if (!glfwInit()) {
		printf("GLFW initialization failed.\n");
	}
	else
		printf("GLFW initialization successful.\n");


	// GLFW INITIALIZATION ----------------------------

	// GLFW WINDOW HINTS ----------------------------------

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// GLFW WINDOW HINTS ----------------------------------

	// GLFW WINDOW SETUP -------------------------------------------------

	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);

	if (!window) {
		printf("Creating GLFW window failed.\n");
	}
	else
		printf("GLFW window successfully created.\n");


	// GLFW WINDOW SETUP -------------------------------------------------

	glfwSwapInterval(1);

	glfwMakeContextCurrent(window);

	return window;
}

void CMCL_InitializeGLAD() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("GLAD functions failed to load.\n");
	}
	else
		printf("GLAD functions successfully loaded.\n");

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback(&MessageCallback, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);

	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
}

// prevents message duplication
int64_t prevMessageID = -1;

// OpenGL debug callback
void APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{

	if ((GLuint)prevMessageID == id)
		return;
	prevMessageID = id;

	const char* source_str = "";
	switch (source) {
		case GL_DEBUG_SOURCE_API: source_str = "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: source_str = "WINDOW SYSTEM";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: source_str = "SHADER COMPILER";
		case GL_DEBUG_SOURCE_THIRD_PARTY: source_str = "THIRD PARTY";
		case GL_DEBUG_SOURCE_APPLICATION: source_str = "APPLICATION";
		case GL_DEBUG_SOURCE_OTHER: source_str = "OTHER";
		default: break;
	}

	const char* type_str = "";
	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR: type_str = "ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_str = "DEPRECATED_BEHAVIOR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: type_str = "UNDEFINED_BEHAVIOR";
		case GL_DEBUG_TYPE_PORTABILITY: type_str = "PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE: type_str = "PERFORMANCE";
		case GL_DEBUG_TYPE_MARKER: type_str = "MARKER";
		case GL_DEBUG_TYPE_OTHER: type_str = "OTHER";
		default: break;
	}

	switch (severity) {
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			printf("OpenGL Debug Notification: %s, %s, %i: %s\n", source_str, type_str, (int)id, (char*)message);
			break;
		case GL_DEBUG_SEVERITY_LOW:
			printf("OpenGL Debug Low Warning: %s, %s, %i: %s\n", source_str, type_str, (int)id, (char*)message);
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			printf("OpenGL Debug Medium Warning: %s, %s, %i: %s\n", source_str, type_str, (int)id, (char*)message);
			break;
		case GL_DEBUG_SEVERITY_HIGH:
			printf("OpenGL Debug Error: %s, %s, %i: %s\n", source_str, type_str, (int)id, (char*)message);
			break;
		default: break;
	}

}