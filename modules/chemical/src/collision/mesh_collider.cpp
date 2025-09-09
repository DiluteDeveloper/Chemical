#include "mesh_collider.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Chemical {

  // Returns min and max values respectively
  std::pair<float, float>
  ProjectVerticesOntoSeparatingAxis(const glm::vec3 &separating_axis,
                                    const std::vector<Vertex> &vertices,
                                    const glm::mat4 &model_mat) {

    float min = glm::dot(separating_axis, vertices[0].position);
    float max = min;

    for (size_t i = 1; i < vertices.size(); i++) {
      glm::vec3 global_vpos =
          model_mat * glm::vec4(vertices[i].position.x, vertices[i].position.y,
                                vertices[i].position.z, 0);
      float value = glm::dot(separating_axis, global_vpos);

      if (value < min)
        min = value;
      else if (value > max)
        max = value;
    }

    return std::make_pair(min, max);
  }

  bool IsColliding_SAT(const std::vector<Vertex> &s0,
                       const glm::mat4 &s0_model_mat,
                       const std::vector<Vertex> &s1,
                       const glm::mat4 &s1_model_mat) {

    for (const Vertex &v0 : s0) {

      auto [s0_min, s0_max] =
          ProjectVerticesOntoSeparatingAxis(v0.normal, s0, s0_model_mat);
      auto [s1_min, s1_max] =
          ProjectVerticesOntoSeparatingAxis(v0.normal, s1, s1_model_mat);

      if (!((s0_min < s1_max && s0_min > s1_min) ||
            (s1_min < s0_max && s1_min > s0_min)))
        return false;
    }
    for (const Vertex &v1 : s1) {
      auto [s0_min, s0_max] =
          ProjectVerticesOntoSeparatingAxis(v1.normal, s0, s0_model_mat);
      auto [s1_min, s1_max] =
          ProjectVerticesOntoSeparatingAxis(v1.normal, s1, s1_model_mat);

      if (!((s0_min < s1_max && s0_min > s1_min) ||
            (s1_min < s0_max && s1_min > s0_min)))
        return false;
    }

    return true;
  }
} // namespace Chemical
