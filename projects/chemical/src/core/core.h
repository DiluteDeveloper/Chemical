#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

/*
Note to self:
Think in terms of engine tools.
Its not an input tool for the user,
its an input tool for the engine.
Its not an window tool for the game,
its an input tool for the engine.
*/

namespace Chemical {
	namespace Core {

		extern GLFWwindow* window; // make a wrapper for this eventually
		extern std::string glsl_version; // and this
		extern glm::ivec2 window_size;
	

	}
}