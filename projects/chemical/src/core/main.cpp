#include <pch.h>
#include <GLFW/glfw3.h>
#include <GLAD/glad.h>

#include "glfw_glad/init_glfw_glad.h"

#include "rendering/renderer.h"
#include "rendering/model_importer.h"

using namespace Chemical;

int main(int argc, char* argv[]) {

	InitializeGLFWGLAD(1280, 720);
	if (QUERY_ERROR) {
		CONSOLE_PRINT(Severity::_ERROR, "Error occurred with GLFW or GLAD initialization.");
		glfwTerminate();
		return 0;
	}

	Renderer3D renderer;
	if (QUERY_ERROR) {
		CONSOLE_PRINT(Severity::_ERROR, "Error occurred with renderer constructor.");
		glfwTerminate();
		return 0;
	}
	ModelImporter importer;
	
	auto m = importer.ImportModel("resources/models/stanford-dragon.fbx");
	if (QUERY_ERROR) {
		CONSOLE_PRINT(Severity::_ERROR, "Error occurred with model import.");
		glfwTerminate();
		return 0;
	}

	renderer.AddMeshToRender(m.value());


	// GLAD PREFERENCES -----------------------------------------

	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.Render();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
	glfwTerminate();
}