#pragma once

#include <unordered_map>
#include <vector>

#include "Shader.h"
#include "Renderable.h"

namespace Chemical {
	namespace Render {

		struct Scene {
			std::unordered_map<Shader*, std::vector<Renderable>> renderables;
		};
	}
}
