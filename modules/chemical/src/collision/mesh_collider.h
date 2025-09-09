#pragma once

#include "rendering/mesh.hpp"
#include <vector>
namespace Chemical {

  // Takes in vertices for two shapes and their model matrix (for global
  // position) and returns whether they are colliding using the Separating Axis
  // Theorem
  bool IsColliding_SAT(const std::vector<Vertex> &s0,
                       const glm::mat4 &s0_model_mat,
                       const std::vector<Vertex> &s1,
                       const glm::mat4 &s1_model_mat);
} // namespace Chemical
