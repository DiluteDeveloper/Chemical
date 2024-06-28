#include <pch.h>

#include "core/core.h"

namespace Chemical {
	namespace Core {
		GLFWwindow* window = nullptr;
		std::string glsl_version = "#version 460";
		glm::ivec2 window_size = glm::ivec2(1280, 720);
	}
}