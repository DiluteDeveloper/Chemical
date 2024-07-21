#include <pch.h>
#include <GLFW/glfw3.h>
#include <GLAD/glad.h>

#include "glfw_glad/init_glfw_glad.h"

#include "rendering/renderer.h"
#include "rendering/model_importer.h"

using namespace Chemical;

int main(int argc, char* argv[]) {

	Util::ConsoleLogger::InitializeLogger();

	InitializeGLFWGLAD(1280, 720);

	Renderer3D::InitializeRenderer();
	ModelImporter::ImportModel("resources/models/stanford-dragon.fbx");

	if (ModelImporter::import_success == true) {
		Renderer3D::AddMeshToRender(ModelImporter::imported_mesh);
	}


	// GLAD PREFERENCES -----------------------------------------

	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Renderer3D::Render();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
	return 0;
}