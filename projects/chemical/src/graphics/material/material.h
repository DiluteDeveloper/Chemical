#pragma once

#include "graphics/opengl/buffer.h"

struct Material {
	glm::fvec4 diffColour = glm::fvec4(1);
	glm::fvec4 specColour = glm::fvec4(1);

	Material(const glm::fvec4& diffColour, const glm::fvec4& specColour)
	: diffColour(diffColour), specColour(specColour) {}
	Material() {}
};
class StaticSSBOMaterial {
	OpenGL::Buffer ssboBuffer;
public:
	StaticSSBOMaterial(const std::vector<Material>& materials) {
		ssboBuffer.CreateImmutableBuffer(sizeof(Material) * materials.size(), materials.data(), OpenGL::BufferStorageFlags::MAP_READ_BIT);
		ssboBuffer.BindBufferBase(OpenGL::BufferBaseTarget::SHADER_STORAGE_BUFFER, 0);
	}
};
