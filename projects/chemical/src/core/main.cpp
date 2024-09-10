#include "pch.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <random>

#include "chunk_system.h"

#include "chunk/chunk.h"
#include "chunk/noise.h"

#include "graphics/opengl/texture.h"

#include <stb_image/stb_image.h>

#include "core.h"

// globals
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


float camSpeed = 0.1f;
float sensitivity = 0.08f;
// movement controls
double oldx, oldy;
void UpdatePlayer() {
	glm::fmat4 matrix = player_transform.GetTransform();
	glm::fvec3 right = matrix[0];
	glm::fvec3 up = matrix[1];
	glm::fvec3 forward = matrix[2];
	glm::fvec3 front = glm::normalize(glm::fvec3(forward.x, 0, forward.z));

	if (glfwGetKey(window, GLFW_KEY_W)) {
		player_transform.position += front * camSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_S)) {
		player_transform.position -= front * camSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_A)) {
		player_transform.position -= right * camSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D)) {
		player_transform.position += right * camSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE)) {
		player_transform.position.y += camSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
		player_transform.position.y -= camSpeed;
	}

	//std::cout << player_transform.position.x << ", " << player_transform.position.y << ", " << player_transform.position.z << std::endl;

	double x, y;
	glfwGetCursorPos(window, &x, &y);

	player_transform.rotation.y += static_cast<float>(x - oldx) * sensitivity;
	player_transform.rotation.x += static_cast<float>(y - oldy) * sensitivity;

	player_transform.rotation.x = glm::clamp(player_transform.rotation.x, -85.0f, 85.0f);

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

	window = glfwCreateWindow(1280, 720, "Chemical", NULL, NULL);

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
	glfwSwapInterval(1);

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
	glFrontFace(GL_CCW);

	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

	// GLAD PREFERENCES -----------------------------------------

	// MORE TESTING CODE --------------------------------------------


	projection = glm::perspectiveLH(glm::radians(90.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);

	std::random_device rd;
	uint32_t seed = rd();

	ChunkLoader loader(seed, 4);

	glfwGetCursorPos(window, &oldx, &oldy);

	int width=0, height=0, channels=0;
	void* dirtdata = stbi_load("resources/textures/blocks/dirt.png", &width, &height, &channels, 0);

	OpenGL::TextureStorageParameters p;
	p.internalFormat = OpenGL::TextureInternalFormat::RGB8;
	p.width = width;
	p.height = height;
	OpenGL::Texture dirt(p);


	OpenGL::TextureDataParameters param;
	param.baseFormat = OpenGL::TextureBaseFormat::RGB;
	param.dataType = OpenGL::DataType::UNSIGNED_BYTE;
	param.width = width;
	param.height = height;
	param.textureType = OpenGL::TextureType::TEXTURE_2D;
	dirt.SetTextureData(param, dirtdata);

	stbi_image_free(dirtdata);

	dirt.GenerateMipmaps();

	dirt.BindTexture(1);

	dirt.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
	dirt.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);


	void* grassdata = stbi_load("resources/textures/blocks/grass.png", &width, &height, &channels, 0);

	OpenGL::Texture grass(p);

	grass.SetTextureData(param, grassdata);

	stbi_image_free(grassdata);

	grass.GenerateMipmaps();

	grass.BindTexture(2);

	grass.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
	grass.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);

	void* stonedata = stbi_load("resources/textures/blocks/stone.png", &width, &height, &channels, 0);

	OpenGL::Texture stone(p);

	stone.SetTextureData(param, stonedata);

	stbi_image_free(stonedata);

	stone.GenerateMipmaps();

	stone.BindTexture(0);

	stone.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
	stone.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);

	void* beddata = stbi_load("resources/textures/blocks/bedrock.png", &width, &height, &channels, 0);

	OpenGL::Texture bed(p);

	bed.SetTextureData(param, beddata);

	stbi_image_free(beddata);

	bed.GenerateMipmaps();

	bed.BindTexture(3);

	bed.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
	bed.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);

	void* logtopdata = stbi_load("resources/textures/blocks/oaklogtop.png", &width, &height, &channels, 0);

	OpenGL::Texture logtop(p);

	logtop.SetTextureData(param, logtopdata);

	stbi_image_free(logtopdata);

	logtop.GenerateMipmaps();

	logtop.BindTexture(4);

	logtop.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
	logtop.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);

	void* logsidedata = stbi_load("resources/textures/blocks/oaklogside.png", &width, &height, &channels, 0);

	OpenGL::Texture logside(p);

	logside.SetTextureData(param, logsidedata);

	stbi_image_free(logsidedata);

	logside.GenerateMipmaps();

	logside.BindTexture(5);

	logside.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
	logside.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);

	// --------------------------

	ChunkData chunk_data;
	ChunkData chunk_data2;

	const siv::PerlinNoise noise_gen{ 505 };

	ChunkHeightMapData chunk_heightmap_data = GenerateChunkHeightMapData(noise_gen, glm::vec2(0, 0));
	ChunkHeightMapData chunk_heightmap_data2 = GenerateChunkHeightMapData(noise_gen, glm::vec2(1, 0));

	GenerateTerrainChunkBlockData(chunk_data.blocks, chunk_heightmap_data);
	GenerateTerrainChunkBlockData(chunk_data2.blocks, chunk_heightmap_data2);

	chunk_data.edges[1] = &chunk_data2;
	chunk_data2.edges[3] = &chunk_data;

	ChunkMeshData chunk_mesh_data = GenerateChunkMeshData(chunk_data, true, 1.2f);
	ChunkMeshData chunk_mesh_data2 = GenerateChunkMeshData(chunk_data2, true, 1.2f);



	// MORE TESTING CODE --------------------------------------------

	while (!glfwWindowShouldClose(window)) {

		glfwMakeContextCurrent(window);

		if(glfwGetKey(window, GLFW_KEY_T))
			loader.RemoveBlock(glm::ivec3(floor(player_transform.position.x), floor(player_transform.position.y), floor(player_transform.position.z)));
		if (glfwGetKey(window, GLFW_KEY_Y)) {
			loader.PlaceBlock(glm::ivec3(floor(player_transform.position.x), floor(player_transform.position.y), floor(player_transform.position.z)));
		}

		if (glfwGetKey(window, GLFW_KEY_6))
			loader.doChunkLoading = false;
		if (glfwGetKey(window, GLFW_KEY_7))
			loader.doChunkLoading = true;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		UpdatePlayer();

		//loader.Update();

		loader.renderer.chunk_shader->BindProgram();
		loader.renderer.chunk_shader->SetUniformMatrix4FV("v_view", 1, false, &glm::inverse(Core::player_transform.GetTransform())[0][0]);

		loader.renderer.chunk_shader->SetUniform2IV("v_chunk_origin", 1, &glm::ivec2(0, 0)[0]);

		chunk_mesh_data.v_array.Bind();
		chunk_mesh_data.v_array.DrawArrays(chunk_mesh_data.info);


		loader.renderer.chunk_shader->SetUniform2IV("v_chunk_origin", 1, &glm::ivec2(16, 0)[0]);

		chunk_mesh_data2.v_array.Bind();
		chunk_mesh_data2.v_array.DrawArrays(chunk_mesh_data2.info);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
	return 0;
}