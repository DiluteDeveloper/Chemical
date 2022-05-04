#include "shader.h"

#include <fstream>
#include <iostream>
#include <vendor/GLAD/glad.h>
#include <glm/gtc/matrix_transform.hpp>

void shader_compile_status(unsigned int shader) {
	int success;
	char info_log[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE) {
		glGetShaderInfoLog(shader, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
	}
}

Shader::Shader(const std::string& vertex_path, const std::string& fragment_path) {

	std::ifstream stream(vertex_path);
	std::string vertex_source((std::istreambuf_iterator<char>(stream)),
		std::istreambuf_iterator<char>());

	stream.close();

	stream.open(fragment_path);
	std::string fragment_source((std::istreambuf_iterator<char>(stream)),
		std::istreambuf_iterator<char>());

	stream.close();

	// Build Shaders

	// VERTEX

	const char* c_vertex_source = vertex_source.c_str();

	unsigned int vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertex_shader, 1, &c_vertex_source, NULL);
	glCompileShader(vertex_shader);

	shader_compile_status(vertex_shader);

	// FRAGMENT

	const char* c_fragment_source = fragment_source.c_str();

	unsigned int fragment_shader;
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragment_shader, 1, &c_fragment_source, NULL);
	glCompileShader(fragment_shader);

	shader_compile_status(fragment_shader);

	id = glCreateProgram();

	glAttachShader(id, vertex_shader);
	glAttachShader(id, fragment_shader);
	glLinkProgram(id);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	glValidateProgram(id);

	int success;
	char info_log[512];
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, info_log);
		std::cout << "ERROR::PROGRAM::COMPILATION_FAILED\n" << info_log << std::endl;
	}
}

Shader::~Shader() {
	glDeleteProgram(id);
}

void Shader::bind() const {
	glUseProgram(id);
}

void Shader::setMatrix4fv(const std::string& name, const glm::mat4& value) const {
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setVector3(const std::string& name, const glm::vec3& value) const {
	glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
}

void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setMaterial(const Material& material) const {
	glUniform3f(glGetUniformLocation(id, "material.diffuse"), material.diffuse.x, material.diffuse.y, material.diffuse.z);
	glUniform3f(glGetUniformLocation(id, "material.specular"), material.specular.x, material.specular.y, material.specular.z);
	glUniform3f(glGetUniformLocation(id, "material.ambient"), material.ambient.x, material.ambient.y, material.ambient.z);
	glUniform1f(glGetUniformLocation(id, "material.shininess"), material.shininess);
}