#include "transform.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Chemical {

  glm::mat4 Transform::ToMatrix() const {
    glm::mat4 m = glm::mat4(1.0f);

    m = glm::translate(m, position);
    m = glm::rotate(m, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    m = glm::rotate(m, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    m = glm::rotate(m, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    m = glm::scale(m, scale);

    return m;
  }
} // namespace Chemical
