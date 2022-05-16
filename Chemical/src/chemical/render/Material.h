#pragma once

#include <vendor/glm/glm.hpp>
#include <filesystem>

#include "Shader.h"

namespace Chemical {

	namespace Render {
		struct Material {
			void Bind(Shader& shader) const {
				shader.SetUniform3FV("material.ambient", ambient);
				shader.SetUniform3FV("material.specular", specular);
				shader.SetUniform3FV("material.diffuse", diffuse);
				shader.SetUniform1FV("material.shininess", shininess);
			}

			glm::fvec3 diffuse = glm::fvec3(1.0f);
			glm::fvec3 specular = glm::fvec3(1.0f);
			glm::fvec3 ambient = glm::fvec3(1.0f);
			float shininess = 32.0f;

			Material() = default;
			~Material() = default;

		};
	}


}
