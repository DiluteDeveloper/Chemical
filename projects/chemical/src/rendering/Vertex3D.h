#pragma once

#include <glm/glm.hpp>

namespace Chemical {
	namespace Rendering {
		struct Vertex3D {
			glm::fvec3 position;
			glm::fvec3 colour;

			Vertex3D(const glm::fvec3& position, const glm::fvec3& colour) :
				position(position), colour(colour) {}
		};
	}
}
