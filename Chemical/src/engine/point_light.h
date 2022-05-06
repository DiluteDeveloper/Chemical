struct Transform;

#include "glm/glm.hpp"

struct PointLight {

	glm::vec3 diffuse = glm::vec3(0.0f);
	glm::vec3 ambient = glm::vec3(0.0f);
	glm::vec3 specular = glm::vec3(0.0f);

	Transform transform;

	PointLight(glm::vec3 diffuse = glm::vec3(0.0f), glm::vec3 ambient = glm::vec3(0.0f), glm::vec3 specular = glm::vec3(0.0f)) : diffuse(diffuse), ambient(ambient), 
		specular(specular) {
		transform.scale = glm::vec3(0.2f);
	};

	bool operator==(const PointLight& p1) const { // might be fucked
		if (transform != p1.transform)
			return false;
		if (diffuse != p1.diffuse)
			return false;
		if (ambient != p1.ambient)
			return false;
		if (specular != p1.specular)
			return false;
		return true;
	}
};