#pragma once

#include <glm/glm.hpp>

struct Material {

	glm::vec3 diffuse = glm::vec3(1.0f);
	glm::vec3 specular = glm::vec3(1.0f);
	glm::vec3 ambient = glm::vec3(1.0f);
	float shininess = 32.0f;

	Material(const glm::vec3& diffuse = glm::vec3(1.0f), const glm::vec3& specular = glm::vec3(1.0f), const glm::vec3& ambient = glm::vec3(1.0f), float shininess = 32.0f)
		: diffuse(diffuse), specular(specular), ambient(ambient), shininess(shininess) {}
};
