#pragma once

#include <glm/glm.hpp>

#include "object.h"
#include "rendering/generated_mesh_3d.h"
#include "modules/transform.h"

namespace Chemical {

	class Mesh3D : public Object {
	public:

		GeneratedMesh3D genMesh;
		Transform transform;

		// std::moves genMesh
		Mesh3D(GeneratedMesh3D&& genMesh) :
				genMesh(std::move(genMesh)) {}

	};

}

