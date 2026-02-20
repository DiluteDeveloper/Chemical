#pragma once

#include "rendering/mesh.hpp"
#include "rendering/shader.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace Chemical {

  class OrbitLineMesh {

    unsigned int segment_count = 0;
    unsigned int vertex_count = 0;

    // The next buffer vertex index (loops to 0 when reaching segment c
    // ount)
    unsigned int buffer_vertex_index = 0;

    // Increases as  buffer is filled up
    unsigned int cur_vertex_count = 0;

    // The last index from position that was added to buffer
    unsigned int position_offset_index = 0;
    unsigned int vao = 0;
    unsigned int vbo = 0;

    bool has_filled_buffer = false;

    Mesh mesh;

  public:
    void Draw(Shader &shader);

    unsigned int GetSegmentCount();
    void SetSegmentCount(unsigned int count);
    void Reset();

    OrbitLineMesh(unsigned int segment_count = 5000);
    ~OrbitLineMesh();
    void Update(const std::vector<glm::vec3> &positions);

    OrbitLineMesh(const OrbitLineMesh &) = delete;
    OrbitLineMesh &operator=(const OrbitLineMesh &) = delete;
    OrbitLineMesh(OrbitLineMesh &&) = delete;
    OrbitLineMesh &operator=(OrbitLineMesh &&) = delete;
  };
} // namespace Chemical
