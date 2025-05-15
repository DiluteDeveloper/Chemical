#include "chemical/graphics/mesh.h"

#include <glad/glad.h>

#include "pch.h"

namespace Chemical {

  namespace Graphics {

    StaticMesh2D CreateStaticMesh2D(const std::vector<float> vertices,
                                    const std::vector<unsigned int> indices) {
      unsigned int VBO = 0;
      glCreateBuffers(1, &VBO);
      glNamedBufferStorage(VBO, sizeof(float) * vertices.size(), &vertices[0],
                           GL_DYNAMIC_STORAGE_BIT);

      unsigned int IBO = 0;
      glCreateBuffers(1, &IBO);
      glNamedBufferStorage(IBO, sizeof(unsigned int) * indices.size(),
                           &indices[0], GL_DYNAMIC_STORAGE_BIT);

      unsigned int VAO = 0;
      glCreateVertexArrays(1, &VAO);

      glVertexArrayVertexBuffer(VAO, 0, VBO, 0, sizeof(float) * 2);
      glVertexArrayElementBuffer(VAO, IBO);

      glEnableVertexArrayAttrib(VAO, 0);

      glVertexArrayAttribFormat(VAO, 0, 2, GL_FLOAT, GL_FALSE, 0);

      glVertexArrayAttribBinding(VAO, 0, 0);

      glDeleteBuffers(1, &VBO);
      glDeleteBuffers(1, &IBO);

      return StaticMesh2D(VAO, indices.size());
    }
    StaticMesh2D CreateStaticMesh2D(Shape2D shape) {
      std::vector<float> vertices;
      std::vector<unsigned int> indices;

      switch (shape) {
      case Shape2D::TRIANGLE:
        vertices = {-0.5f, -0.5f, 0.0f, 0.5f, 0.5f, -0.5f};
        indices = {0, 1, 2};
        break;
      case Shape2D::SQUARE:
        vertices = {-0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f};
        indices = {0, 1, 2, 3, 0, 2};
        break;
      }

      unsigned int VBO = 0;
      glCreateBuffers(1, &VBO);
      glNamedBufferStorage(VBO, sizeof(float) * vertices.size(), &vertices[0],
                           GL_DYNAMIC_STORAGE_BIT);

      unsigned int IBO = 0;
      glCreateBuffers(1, &IBO);
      glNamedBufferStorage(IBO, sizeof(unsigned int) * indices.size(),
                           &indices[0], GL_DYNAMIC_STORAGE_BIT);

      unsigned int VAO = 0;

      glCreateVertexArrays(1, &VAO);

      glVertexArrayVertexBuffer(VAO, 0, VBO, 0, sizeof(float) * 2);
      glVertexArrayElementBuffer(VAO, IBO);

      glEnableVertexArrayAttrib(VAO, 0);

      glVertexArrayAttribFormat(VAO, 0, 2, GL_FLOAT, GL_FALSE, 0);

      glVertexArrayAttribBinding(VAO, 0, 0);

      glDeleteBuffers(1, &VBO);
      glDeleteBuffers(1, &IBO);

      return StaticMesh2D(VAO, indices.size());
    }
    StaticMesh2D::~StaticMesh2D() { glDeleteVertexArrays(1, &VAO); }

    void StaticMesh2D::Draw() {
      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, nullptr);
    }
  } // namespace Graphics
} // namespace Chemical
