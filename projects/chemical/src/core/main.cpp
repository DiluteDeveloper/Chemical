#include "pch.h"

#include "core.h"
#include "gui/gui.h"
#include "gui/surface_gui.h"
#include "graphics/opengl/shader_program.h"
#include "util/filestream.h"
#include "nodes/sprite_2D.h"

using namespace Chemical;

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

int main(int argc, char* argv[]) {

#ifdef CHEMICAL_DEBUG
	Util::Logger::InitializeLogger();
#endif

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

	Core::window = glfwCreateWindow(1280, 720, "Chemical", NULL, NULL);

	if (!Core::window) {
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

	glfwMakeContextCurrent(Core::window);

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
	glDebugMessageCallback(&message_callback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

	// GLAD PREFERENCES -----------------------------------------


	Node::Sprite2D::InitializeSprite2D();
	Node::Sprite2D sprite;

	GUI::InitializeGUI();

	GUI::SurfaceGUI surfaceGUI;

	OpenGL::Shader vShader(Util::ReadFile("resources/shaders/sprite_2D.vert").c_str(), OpenGL::ShaderType::VERTEX_SHADER);
	OpenGL::Shader fShader(Util::ReadFile("resources/shaders/sprite_2D.frag").c_str(), OpenGL::ShaderType::FRAGMENT_SHADER);

	OpenGL::ShaderProgram shaderProgram = {{&vShader, &fShader}, Node::Sprite2D::layout };
	shaderProgram.BindProgram();

	shaderProgram.SetUniform2FV("real_position", 1, &sprite.node_2D.position[0]);
	shaderProgram.SetUniform1UI("zIndex", sprite.zIndex);
	shaderProgram.SetUniform2FV("scale", 1, &sprite.node_2D.scale[0]);

	// MORE TESTING CODE --------------------------------------------

	while (!glfwWindowShouldClose(Core::window)) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GUI::NewFrame();

		surfaceGUI.Update();

		Node::Sprite2D::vArray->Bind();
		Node::Sprite2D::vArray->DrawElements(Node::Sprite2D::info);

		GUI::Render();

		glfwSwapBuffers(Core::window);

		glfwPollEvents();
	}
	return 0;
}