#include "mesh.h"

#include <vendor/GLAD/glad.h>
#include <iostream>

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& in_indices) {
	indices = (unsigned int)in_indices.size();

	glCreateVertexArrays(1, &vao);
	glCreateBuffers(2, &buffers[0]);



	glNamedBufferStorage(buffers[0], sizeof(float) * vertices.size(), &vertices[0], GL_MAP_READ_BIT);
	glNamedBufferStorage(buffers[1], sizeof(unsigned int) * indices, &in_indices[0], GL_MAP_READ_BIT);

	glVertexArrayVertexBuffer(vao, 0, buffers[0], 0, sizeof(float) * 8);
	glVertexArrayElementBuffer(vao, buffers[1]);

	glEnableVertexArrayAttrib(vao, 0);
	glEnableVertexArrayAttrib(vao, 1);
	glEnableVertexArrayAttrib(vao, 2);

	glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3);
	glVertexArrayAttribFormat(vao, 2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5);

	glVertexArrayAttribBinding(vao, 0, 0);
	glVertexArrayAttribBinding(vao, 1, 0);
	glVertexArrayAttribBinding(vao, 2, 0);

}

void Mesh::draw() const {
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(2, &buffers[0]);
}
