#pragma once

#include "Mesh.h"
#include "Material.h"
#include "chemical/util/Transform.h"

namespace Chemical {

	namespace Render {

		struct Renderable {

			Material material;
			Mesh* mesh;
			Util::Transform transform;

			Renderable(Mesh& mesh, const Material& material) : mesh(&mesh), material(material) {};
		};
	}
}
