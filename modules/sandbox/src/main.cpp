#include <chemical/window.h>
#include <chemical/core.h>
#include <chemical/graphics/misc.h>
#include <chemical/graphics/mesh.h>

using namespace Chemical;

int main() {
	InitialiseChemical();

	std::optional<Window> window = CreateNewWindow("Chemical 1.1.1", 1280, 720);

	if (!window.has_value())
		return -1;

	Graphics::RGBColour background(30, 60, 85);

	Graphics::SetBackgroundColour(background);

	const std::vector<float> vertices = {
		-0.5f, -0.5f,
		0.0f, 0.5f,
		0.5f, -0.5f
	};
	const std::vector<unsigned int> indices = {
		0,1,2
	};

	Graphics::StaticMesh2D mesh = Graphics::CreateStaticMesh2D(vertices, indices);

	while (!WindowShouldClose(window.value())) {
		PollEvents();

		Graphics::ClearColourBuffer();

		mesh.Draw();

		Graphics::SwapBuffers(window.value());
	}
}