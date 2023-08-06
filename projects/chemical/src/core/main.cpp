#include "pch.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <random>


#include "transform/transform.h"

#include "util/filestream.h"
#include "graphics/opengl/shader_program.h"
#include "graphics/opengl/buffer.h"
#include "graphics/opengl/vertex_array.h"

#include "chunk_system.h"

#include "core.h"

namespace Chemical {
	using namespace Core;
	namespace Core {

		Transform player_transform = Transform{};
		glm::mat4 projection = glm::mat4{};

		GLFWwindow* window = nullptr;
	}
}

using namespace Chemical;

#ifdef CHEMICAL_DEBUG
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
#endif

double oldx, oldy;
void UpdatePlayer() {
	glm::fmat4 matrix = player_transform.GetTransform();
	glm::fvec3 forward = matrix[2];
	glm::fvec3 front = glm::normalize(glm::vec3(forward.x, 0, forward.z));
	glm::fvec3 right = matrix[0];

	if (glfwGetKey(window, GLFW_KEY_W)) {
		player_transform.position -= front * 0.15f;
	}
	if (glfwGetKey(window, GLFW_KEY_S)) {
		player_transform.position += front * 0.15f;
	}
	if (glfwGetKey(window, GLFW_KEY_A)) {
		player_transform.position -= right * 0.15f;
	}
	if (glfwGetKey(window, GLFW_KEY_D)) {
		player_transform.position += right * 0.15f;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE)) {
		player_transform.position.y += 0.15f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
		player_transform.position.y -= 0.15f;
	}

	double x, y;
	glfwGetCursorPos(window, &x, &y);

	player_transform.rotation.y -= static_cast<float>(x - oldx) * 0.002f;
	player_transform.rotation.x -= static_cast<float>(y - oldy) * 0.002f;

	player_transform.rotation.x = glm::clamp(player_transform.rotation.x, -80.0f, 80.0f);

	oldx = x;
	oldy = y;
}

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

	window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);

	if (!window) {
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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLFW PREFERENCES ----------------------------------

	// GLAD SETUP --------------------------------------------

	glfwMakeContextCurrent(window);

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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

	// GLAD PREFERENCES -----------------------------------------

	// MORE TESTING CODE --------------------------------------------

	projection = glm::perspective(glm::radians(90.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);

	InitializeBlockData();

	std::random_device rd;
	uint32_t seed = rd();

	std::unique_ptr<ChunkLoader> loader = ChunkLoader::CreateChunkLoader(32, seed);

	// MORE TESTING CODE --------------------------------------------

	while (!glfwWindowShouldClose(window)) {

		glfwMakeContextCurrent(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		UpdatePlayer();

		loader->Update();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
	return 0;
}