#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Chemical {
  struct Renderable {
    glm::uvec3 tint = glm::uvec3(255, 255, 255);
    glm::vec2 size = glm::vec2(1.0f);
    glm::mat3 model = glm::mat3(1.0f);
    glm::mat3 view = glm::mat3(1.0f);
    glm::mat3 proj = glm::mat3(1.0f);

    std::string shader_id = "";
    std::string texture_id = "";

    Renderable(const std::string& shader_id, const std::string& texture_id)
        : shader_id(shader_id), texture_id(texture_id) {}
  };

} // namespace Chemical
