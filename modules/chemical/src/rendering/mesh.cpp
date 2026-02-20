
#include "mesh.hpp"
#include <spdlog/spdlog.h>

#include "glad/glad.h"

namespace Chemical {

  unsigned int Mesh::AsVAO() {
    unsigned int vao = 0, vbo = 0, ibo = 0;
    if (indices.size() == 0) {
      for (unsigned int i = 0; i < vertices.size(); i++) {
        indices.emplace_back(i);
      }
    }
    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vbo);
    glCreateBuffers(1, &ibo);

    glNamedBufferStorage(vbo, sizeof(float) * vertices.size() * 6, &vertices[0],
                         GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(ibo, sizeof(unsigned int) * indices.size(),
                         &indices[0], GL_DYNAMIC_STORAGE_BIT);

    glVertexArrayVertexBuffer(vao, 0, vbo, 0, 6 * sizeof(float));
    glVertexArrayElementBuffer(vao, ibo);

    glVertexArrayAttribBinding(vao, 0, 0);
    glVertexArrayAttribBinding(vao, 1, 0);

    glEnableVertexArrayAttrib(vao, 0);
    glEnableVertexArrayAttrib(vao, 1);

    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    return vao;
  }
} // namespace Chemical
