#pragma once

#include "Transform.h"

namespace Chemical {
	class Mesh;
	struct Material;
	class Shader;

	struct Renderable {

		Transform transform;
		Material* material;
		Mesh* mesh;
		Shader* shader;

	};
}
