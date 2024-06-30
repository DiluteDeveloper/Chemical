#include "pch.h"

#include "window.h"
#include "gui/gui.h"
#include "gui/surface_gui.h"
#include "graphics/opengl/shader_program.h"
#include "util/filestream.h"

#include "core/sprite_renderer.h"
using namespace Chemical;

int main(int argc, char* argv[]) {

#ifdef CHEMICAL_DEBUG
	Util::Logger::InitializeLogger();
#endif

	Core::Window::InitializeWindow();
	GUI::InitializeGUI();

	Core::SpriteRenderer sprite_renderer;

	std::vector<Node::Sprite2D> sprites;

	sprites.emplace_back();
	sprites.emplace_back();

	sprites[0].SetImageTexture(Util::LoadImageFromPath("resources/textures/test.png", 4));
	sprites[0].node_2d.SetPosition(-500, 0);
	sprites[1].SetImageTexture(Util::LoadImageFromPath("resources/textures/blocks/dirt.png", 4));
	sprites[1].node_2d.SetPosition(500, 0);

	GUI::SurfaceGUI surface_gui;

	// MORE TESTING CODE --------------------------------------------

	while (!Core::Window::WindowShouldClose()) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GUI::NewFrame();

		surface_gui.Update();

		sprite_renderer.RenderSprites(sprites);

		GUI::Render();

		Core::Window::SwapBuffers();

		glfwPollEvents();
	}
	return 0;
}