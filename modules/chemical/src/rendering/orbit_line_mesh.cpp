#include "orbit_line_mesh.hpp"
#include "spdlog/spdlog.h"

#include <cstdlib>
#include <glad/glad.h>

namespace Chemical {

  constexpr unsigned int vertex_byte_size = sizeof(float) * 6;

  OrbitLineMesh::OrbitLineMesh(unsigned int segment_count)
      : segment_count(segment_count), vertex_count(segment_count + 1) {
    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vbo);

    glNamedBufferStorage(vbo, vertex_byte_size * vertex_count, nullptr,
                         GL_DYNAMIC_STORAGE_BIT);

    glVertexArrayVertexBuffer(vao, 0, vbo, 0, vertex_byte_size);

    glVertexArrayAttribBinding(vao, 0, 0);
    glVertexArrayAttribBinding(vao, 1, 0);

    glEnableVertexArrayAttrib(vao, 0);
    glEnableVertexArrayAttrib(vao, 1);

    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
  }
  OrbitLineMesh::~OrbitLineMesh() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
  }

  void OrbitLineMesh::Update(const std::vector<glm::vec3> &positions) {
    unsigned int num_new_positions =
        (positions.size() - 1) - position_offset_index;

    // Early return if not enough positions to add
    if (positions.size() <= 1 || num_new_positions == 0)
      return;

    for (unsigned int i = 0; i < num_new_positions; i++) {

      const glm::vec3 &position = positions.at(position_offset_index);
      // SPDLOG_INFO("POS: {} {} {}", position.x, position.y, position.z);
      unsigned int num_vertex_spots_left_in_buffer =
          (vertex_count)-buffer_vertex_index;
      // SPDLOG_INFO("Vertex count: {}", vertex_count);

      if (num_vertex_spots_left_in_buffer == 0) {
        buffer_vertex_index =
            0; // start overwriting vertices at start of buffer
        has_filled_buffer = true;
      }

      Vertex vtx = Vertex(position);

      glNamedBufferSubData(vbo, buffer_vertex_index * vertex_byte_size,
                           vertex_byte_size, &vtx);
      // SPDLOG_INFO("buffer size: {}", vertex_count * vertex_byte_size);
      // SPDLOG_INFO("buffer byte position offset: {}",
      //             buffer_vertex_index * vertex_byte_size);
      // SPDLOG_INFO("vertex byte size: {}", vertex_byte_size);
      // SPDLOG_INFO("cur_vertex_count: {}", cur_vertex_count);
      // SPDLOG_INFO("num_new_positions: {}", num_new_positions);
      // SPDLOG_INFO("num_vertex_spots_left_in_buffer: {}",
      //             num_vertex_spots_left_in_buffer);
      // SPDLOG_INFO("buffer_vertex_index: {}", buffer_vertex_index);
      // SPDLOG_INFO("position_offset_index: {}", position_offset_index);
      //
      buffer_vertex_index++;
      position_offset_index++;
    }
  }
  void OrbitLineMesh::Draw(Shader &shader) {
    glBindVertexArray(vao);
    shader.SetUniformMatrix4FV("v_model", 1, GL_FALSE, &glm::mat4(1.0f)[0][0]);
    glDrawArrays(GL_LINE_STRIP, 0,
                 std::max(buffer_vertex_index, (unsigned int)1));
    if (has_filled_buffer)
      glDrawArrays(GL_LINE_STRIP, buffer_vertex_index,
                   (vertex_count)-buffer_vertex_index);
  }

  unsigned int OrbitLineMesh::GetSegmentCount() {
    return segment_count;
  }
  void OrbitLineMesh::SetSegmentCount(unsigned int count) {
    position_offset_index =
        std::max((int)0, (int)position_offset_index - (int)buffer_vertex_index);
    segment_count = count;
    vertex_count = segment_count + 1;
    buffer_vertex_index = 0;
    cur_vertex_count = 0;
    has_filled_buffer = false;

    glDeleteBuffers(1, &vbo);
    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, vertex_byte_size * vertex_count, nullptr,
                         GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayVertexBuffer(vao, 0, vbo, 0, vertex_byte_size);
  }
  void OrbitLineMesh::Reset() {
    position_offset_index = 0;
    buffer_vertex_index = 0;
    cur_vertex_count = 0;
    has_filled_buffer = false;
  }
} // namespace Chemical
