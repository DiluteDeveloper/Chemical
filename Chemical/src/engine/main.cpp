#include "window.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "time.h"
#include "game.h"

#include <GLFW/glfw3.h>
#include <vendor/GLAD/glad.h>
#include <stb_image/stb_image.h>
#define STB_IMAGE_IMPLEMENTATION

#include <vendor/IMGUI/imgui.h>
#include <vendor/IMGUI/imgui_impl_opengl3.h>
#include <vendor/IMGUI/imgui_impl_glfw.h>

int main() {

	Window::init();
    Window::initGUI();
    Time::init();

    stbi_set_flip_vertically_on_load(true);

    Game game;

	while (!Window::close()) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Time::update();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        game.update();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		Window::swapBuffers();

		glfwPollEvents();
	}

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}