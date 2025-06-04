#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Chemical {

  using ObjectID = std::string;

  namespace Graphics {

    // Represents an OpenGL vertex array object and its functionality
    // Deleted copy ctor & operator, custom move ctor and operator
    // Not meant to be directly accessed by user
    class StaticSprite {
    public:
      // Generates the OpenGL vertex array object
      StaticSprite(const ObjectID &transform_id, const ObjectID &material_id,
                   glm::vec2 size = glm::vec2(1.0f));

      // Deletes the OpenGL vertex array object
      ~StaticSprite();

      StaticSprite(StaticSprite &&other)
          : vao(other.vao), transform_id(other.transform_id), material_id(other.material_id) {
        other.vao = 0;
      }
      StaticSprite &operator=(StaticSprite &&other) {
        vao = other.vao;
        other.vao = 0;
        material_id = other.material_id;
        transform_id = other.transform_id;
        return *this;
      }

      StaticSprite(const StaticSprite &other) = delete;
      StaticSprite &operator=(const StaticSprite &other) = delete;

      ObjectID material_id = "default";
      ObjectID transform_id = "default";

      void Draw() const;

      unsigned int vao = 0;

      static constexpr unsigned int idx_count = 6;
    };

  } // namespace Graphics
} // namespace Chemical
