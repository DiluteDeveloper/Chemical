#pragma once

#include <vendor/glm/glm.hpp>

namespace Chemical {
	struct Transform {
		glm::fvec3 position = glm::fvec3(0.0f);
		glm::fvec3 rotation = glm::fvec3(0.0f);
		glm::fvec3 scale = glm::fvec3(1.0f);

		Transform(glm::fvec3 position = glm::fvec3(0.0f), glm::fvec3 rotation = glm::fvec3(0.0f), glm::fvec3 scale = glm::fvec3(1.0f));

		glm::fmat4 ToMat4() const;

	};

}
