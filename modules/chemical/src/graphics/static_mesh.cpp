#include "chemical/graphics/static_mesh.h"

#include "chemical/graphics/shader.h"
#include "spdlog/spdlog.h"

#include <glad/glad.h>

namespace Chemical {

  namespace Graphics {
    StaticMeshTraits::StaticMeshTraits(Shape shape, const ObjectID &transform_id, const ObjectID &material_id)
        : transform_id(transform_id), material_id(material_id) {

      switch (shape) {
      case Shape::TRIANGLE:
        SPDLOG_INFO("Initialising vertices and indices for Shape::TRIANGLE");
        vertices = {-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f};
        indices = {0, 1, 2};
        break;
      case Shape::SQUARE:
        SPDLOG_INFO("Initialising vertices and indices for Shape::SQUARE");
        vertices = {-1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f,  0.0f, 1.0f,
                    1.0f,  1.0f,  1.0f, 1.0f, 1.0f,  -1.0f, 1.0f, 0.0f};
        indices = {0, 1, 2, 3, 0, 2};
        break;
      }
    }

    StaticMeshTraits::StaticMeshTraits(const ObjectID &transform_id, const ObjectID &material_id)
        : transform_id(transform_id), material_id(material_id) {}

    StaticMesh::StaticMesh(const Graphics::StaticMeshTraits &traits)
        : idx_count(traits.indices.size()), material_id(traits.material_id),
          transform_id(traits.transform_id) {

      unsigned int VBO = 0;
      glCreateBuffers(1, &VBO);
      glNamedBufferStorage(VBO, sizeof(float) * traits.vertices.size(), &traits.vertices[0],
                           GL_DYNAMIC_STORAGE_BIT);

      unsigned int IBO = 0;
      glCreateBuffers(1, &IBO);
      glNamedBufferStorage(IBO, sizeof(unsigned int) * traits.indices.size(), &traits.indices[0],
                           GL_DYNAMIC_STORAGE_BIT);

      glCreateVertexArrays(1, &vao);
      SPDLOG_INFO(R"(Initialising OpenGL vertex array "{}")", vao);

      glVertexArrayVertexBuffer(vao, 0, VBO, 0, sizeof(float) * 4);
      glVertexArrayElementBuffer(vao, IBO);

      glEnableVertexArrayAttrib(vao, 0);
      glEnableVertexArrayAttrib(vao, 1);

      glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
      glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2);

      glVertexArrayAttribBinding(vao, 0, 0);
      glVertexArrayAttribBinding(vao, 1, 0);

      // Remove these when graphics debugging
      glDeleteBuffers(1, &VBO);
      glDeleteBuffers(1, &IBO);
    }

    void StaticMesh::Draw() const {
      glBindVertexArray(vao);
      glDrawElements(GL_TRIANGLES, idx_count, GL_UNSIGNED_INT, nullptr);
    }
    StaticMesh::~StaticMesh() {
      SPDLOG_INFO(R"(Deleting OpenGL vertex array "{}")", vao);
      glDeleteVertexArrays(1, &vao);
    }

  } // namespace Graphics

} // namespace Chemical
