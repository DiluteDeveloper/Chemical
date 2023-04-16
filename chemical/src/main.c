#include "pch.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "types/string.h"


int64_t prevMessageID = -1;
void APIENTRY message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{

	if ((GLuint)prevMessageID == id)
		return;
	prevMessageID = id;

#ifdef false
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

#endif

}

extern void on_main_begin();
extern void on_chemical_begin();
extern void on_scene_begin();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
		on_scene_begin();
}



int main(int argc, char* argv[]) {
	on_main_begin();

	//Logger::InitializeLogger();

	if (!glfwInit()) {
		//LOGGER_CONSOLE_ERROR("GLFW initialization failed.");
		return -1;
	}
	//else
		//LOGGER_CONSOLE_MESSAGE("GLFW initialized.");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);

	if (!window) {
		//LOGGER_CONSOLE_ERROR("GLFW window creation failed.");
		return -1;
	}
	//else
		//LOGGER_CONSOLE_MESSAGE("GLFW window created.");
	glfwMakeContextCurrent(window);

	if (!gladLoadGL()) {
		//LOGGER_CONSOLE_ERROR("gladLoadGL failed.");
		return -1;
	}
	//else
		//LOGGER_CONSOLE_MESSAGE("gladLoadGL succeeded.");

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glEnable(GL_DEPTH_TEST);
	glDebugMessageCallback(&message_callback, NULL);
	//glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

	glfwSetKeyCallback(window, key_callback);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	//glfwSwapInterval(1);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

	string s = string_create();
	string s2 = string_create();

	string_append_c("HELLO", &s);
	for (size_t i = 0; i < 5000000; i++)
	{
		string_append_c(" HELLO ", &s);
	}
	string_append_c("WORLD!", &s2);
	string_append_s(&s2, &s);

	//printf("%s", s.data);

	string_delete(&s);

	on_chemical_begin();

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwSwapBuffers(window);
	}
	return 0;
}