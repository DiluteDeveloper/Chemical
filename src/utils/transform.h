#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Chemical {

    namespace Utils {

        struct Transform {
            glm::vec3 position = glm::vec3(0.0f);
            glm::vec3 rotation = glm::vec3(0.0f);
            glm::vec3 scale = glm::vec3(1.0f);

            glm::fmat4 AsMat4() const {

                glm::mat4 m = glm::translate(glm::mat4(1.0f), position);
                m = glm::rotate(m, glm::radians(rotation.z), glm::vec3(0, 0, 1));
                m = glm::rotate(m, glm::radians(rotation.y), glm::vec3(0, 1, 0));
                m = glm::rotate(m, glm::radians(rotation.x), glm::vec3(1, 0, 0));
                m = glm::scale(m, scale);
                return m;
            }
        };
    }

}
