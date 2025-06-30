#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

namespace Chemical {

  struct Transform {
    glm::vec2 position = glm::vec2(0.0f);
    glm::vec2 scale = glm::vec2(1.0f);
    float rotation = 0.0f;

    Transform(const glm::vec2& position = glm::vec2(0.0f), float rotation = 0.0f,
              const glm::vec2& scale = glm::vec2(1.0f))
        : position(position), rotation(rotation), scale(scale) {}
    glm::mat3 CreateMat3() const {

      glm::mat3 ret(1.0f);
      ret = glm::translate(ret, position);
      ret = glm::rotate(ret, glm::radians(rotation));
      ret = glm::scale(ret, scale);
      return ret;
    }
  };

} // namespace Chemical
