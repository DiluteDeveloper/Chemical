#include "Mesh.h"

#include <glad/glad.h>
#include <iostream>

namespace Chemical {
	Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& in_indices, const std::string& filePath) : filePath(filePath) {
		indices = (unsigned int)in_indices.size();

		glCreateVertexArrays(1, &VAO);
		glCreateBuffers(2, &buffers[0]);



		glNamedBufferStorage(buffers[0], sizeof(float) * vertices.size(), &vertices[0], GL_MAP_READ_BIT);
		glNamedBufferStorage(buffers[1], sizeof(unsigned int) * indices, &in_indices[0], GL_MAP_READ_BIT);

		glVertexArrayVertexBuffer(VAO, 0, buffers[0], 0, sizeof(float) * 8);
		glVertexArrayElementBuffer(VAO, buffers[1]);

		glEnableVertexArrayAttrib(VAO, 0);
		glEnableVertexArrayAttrib(VAO, 1);
		glEnableVertexArrayAttrib(VAO, 2);

		glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3);
		glVertexArrayAttribFormat(VAO, 2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5);

		glVertexArrayAttribBinding(VAO, 0, 0);
		glVertexArrayAttribBinding(VAO, 1, 0);
		glVertexArrayAttribBinding(VAO, 2, 0);

	}

	void Mesh::Draw() const {
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);
	}
}
