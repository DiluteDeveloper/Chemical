#pragma once

#include <glm/glm.hpp>

#include "transform/transform.h"

struct GLFWwindow;

namespace Chemical {
	namespace Core {
		extern Transform player_transform;
		extern glm::mat4 projection;

		extern GLFWwindow* window;
	
	}
}