#include <GLFW/glfw3.h>

#include "Engine.h"

/*

TODO:

Logging and logging colour options, probably static
AssetImporter for loading in assets such as 3d models
Scripting
GUI

*/

int main() {

	Engine engine;

	while (!glfwWindowShouldClose(engine.window)) {
		engine.Update();
	}
	return 0;
}