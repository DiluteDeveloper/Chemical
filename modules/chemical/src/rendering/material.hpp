#pragma once

#include <cstdint>
#include <format>
#include <string>

#include "glm/glm.hpp"
namespace Chemical {

  struct Material {
    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuse = glm::vec3(0.7f, 0.7f, 0.7f);
    glm::vec3 specular = glm::vec3(1.0, 1.0, 1.0);
    uint16_t shininess = 32;

    std::string ToString() {
      return std::format("\n([Ambient: {}, {}, {}], \n[Diffuse: {}, {}, {}], "
                         "\n[Specular: {}, {}, {}], \n[Shininess: {}])",
                         ambient.x, ambient.y, ambient.z, diffuse.x, diffuse.y,
                         diffuse.z, specular.x, specular.y, specular.z,
                         shininess);
    }
  };
} // namespace Chemical
