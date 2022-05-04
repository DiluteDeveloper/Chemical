#pragma once

#include <string>
#include <glm/glm.hpp>

#include "material.h"

class Shader {

	unsigned int id = 0;

public:
	~Shader();

	Shader(const std::string& vertex_path, const std::string& fragment_path);


	void bind() const;

	void setMatrix4fv(const std::string& name, const glm::mat4& value) const;
	void setVector3(const std::string& name, const glm::vec3& value) const;
	void setFloat(const std::string& name, float value) const;
	void setMaterial(const Material& material) const;
};
