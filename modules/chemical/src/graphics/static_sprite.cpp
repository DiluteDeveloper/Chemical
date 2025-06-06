#include "chemical/graphics/static_sprite.h"

#include "spdlog/spdlog.h"

#include <glad/glad.h>

namespace Chemical {

  namespace Graphics {

    StaticSprite::StaticSprite(const ObjectID &transform_id, const ObjectID &material_id, glm::vec2 size)
        : material_id(material_id), transform_id(transform_id) {

      const float vertices[] = {
          -size.x / 2.0f, -size.y / 2.0f, 0.0f, 0.0f, -size.x / 2.0f, size.y / 2.0f,  0.0f, 1.0f,
          size.x / 2.0f,  size.y / 2.0f,  1.0f, 1.0f, size.x / 2.0f,  -size.y / 2.0f, 1.0f, 0.0f};
      constexpr unsigned int indices[] = {0, 1, 2, 2, 0, 3};

      unsigned int VBO = 0;
      glCreateBuffers(1, &VBO);
      glNamedBufferStorage(VBO, sizeof(float) * 16, &vertices[0], GL_MAP_READ_BIT);

      unsigned int IBO = 0;
      glCreateBuffers(1, &IBO);
      glNamedBufferStorage(IBO, sizeof(unsigned int) * 6, &indices[0], GL_DYNAMIC_STORAGE_BIT);

      glCreateVertexArrays(1, &vao);

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

    void StaticSprite::Draw() const {
      glBindVertexArray(vao);
      glDrawElements(GL_TRIANGLES, idx_count, GL_UNSIGNED_INT, nullptr);
    }
    StaticSprite::~StaticSprite() {
      SPDLOG_INFO(R"(Deleting OpenGL vertex array "{}")", vao);
      glDeleteVertexArrays(1, &vao);
    }

  } // namespace Graphics

} // namespace Chemical
