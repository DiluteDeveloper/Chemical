#include "mesh_collider.h"
#include "spdlog/spdlog.h"

#include <glm/gtc/matrix_transform.hpp>

#include <unordered_set>

namespace Chemical {

  // Returns min and max values respectively
  std::pair<float, float>
  ProjectVerticesOntoAxis(const glm::vec3 &separating_axis,
                          const std::vector<Vertex> &vertices,
                          const glm::mat4 &model_mat) {

    glm::vec3 global_vpos =
        model_mat * glm::vec4(vertices[0].position.x, vertices[0].position.y,
                              vertices[0].position.z, 1);
    float min = glm::dot(separating_axis, global_vpos);
    float max = min;

    for (size_t i = 1; i < vertices.size(); i++) {
      global_vpos =
          model_mat * glm::vec4(vertices[i].position.x, vertices[i].position.y,
                                vertices[i].position.z, 1);
      float value = glm::dot(separating_axis, global_vpos);

      if (value < min)
        min = value;
      else if (value > max)
        max = value;
    }

    return std::make_pair(min, max);
  }

  float normal_hash(const glm::vec3 &normal) {
    return normal.x + (normal.y * 10) + (normal.z * 100);
  }

  // bool glm::vec3::operator<(const glm::vec3& rhs) const {
  // }
  bool IsColliding_SAT(const std::vector<Vertex> &s0,
                       const glm::mat4 &s0_model_mat,
                       const std::vector<Vertex> &s1,
                       const glm::mat4 &s1_model_mat) {

    std::unordered_set<float> normal_hashes_projected;

    for (const Vertex &v0 : s0) {

      if (normal_hashes_projected.contains(normal_hash(v0.normal)))
        continue;
      normal_hashes_projected.emplace(normal_hash(v0.normal));

      auto [s0_min, s0_max] =
          ProjectVerticesOntoAxis(v0.normal, s0, s0_model_mat);
      auto [s1_min, s1_max] =
          ProjectVerticesOntoAxis(v0.normal, s1, s1_model_mat);

      if (s0_max < s1_min || s1_max < s0_min)
        return false;
    }

    for (const Vertex &v1 : s1) {
      if (normal_hashes_projected.contains(normal_hash(v1.normal)))
        continue;
      normal_hashes_projected.emplace(normal_hash(v1.normal));

      auto [s0_min, s0_max] =
          ProjectVerticesOntoAxis(v1.normal, s0, s0_model_mat);
      auto [s1_min, s1_max] =
          ProjectVerticesOntoAxis(v1.normal, s1, s1_model_mat);

      if (s0_max < s1_min || s1_max < s0_min)
        return false;
    }

    return true;
  }
} // namespace Chemical
