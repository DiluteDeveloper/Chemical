#include "pch.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "util/logger.h"
#include "opengl/vertex_array.h"


int64_t prevMessageID = -1;
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



int main(int argc, char* argv[]) {

	Logger::InitializeLogger();

	if (!glfwInit()) {
		LOGGER_CONSOLE_ERROR("GLFW initialization failed.");
		throw std::exception();
	}
	else
		LOGGER_CONSOLE_MESSAGE("GLFW initialized.");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);

	if (!window) {
		LOGGER_CONSOLE_ERROR("GLFW window creation failed.");
		throw std::exception();
	}
	else
		LOGGER_CONSOLE_MESSAGE("GLFW window created.");
	glfwMakeContextCurrent(window);

	if (!gladLoadGL()) {
		LOGGER_CONSOLE_ERROR("gladLoadGL failed.");
		throw std::exception();
	}
	else
		LOGGER_CONSOLE_MESSAGE("gladLoadGL succeeded.");

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glEnable(GL_DEPTH_TEST);
	glDebugMessageCallback(&message_callback, nullptr);
	//glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	//glfwSwapInterval(1);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		std::vector<float> vertices = {
		-0.5f, -0.5f,
		0.0f, 0.5f,
		0.5f, -0.5f
		};
		OpenGL::Buffer vertexBuffer;
		vertexBuffer.CreateImmutableBuffer(sizeof(float) * vertices.size(), vertices.data());
		OpenGL::VertexBufferInfo info(sizeof(float) * 2, 0, 0);

		OpenGL::VertexAttribute attribute(2, 0);
		info.AddAttribute(attribute);

		std::vector<unsigned char> indices = {
			0,1,2
		};
		OpenGL::Buffer elementBuffer;
		elementBuffer.CreateImmutableBuffer(sizeof(unsigned int) * indices.size(), indices.data());
		
		OpenGL::VertexArray vertexArray;
		vertexArray.SetVertexBuffer(vertexBuffer, info);
		vertexArray.SetElementBuffer(elementBuffer);

		vertexArray.Bind();
		OpenGL::ElementDrawInfo drawInfo(indices.size(), 0, OpenGL::DataType::UNSIGNED_BYTE, OpenGL::DrawMode::TRIANGLES);
		vertexArray.DrawElements(drawInfo);

		glfwSwapBuffers(window);
	}
	return 0;
}