#pragma once

#include "chemical/util/transform.h"
#include "material.h"
#include "shader.h"

#include <vector>

namespace Chemical {

  namespace Graphics {

    class StaticMesh;

    enum class Shape { SQUARE, TRIANGLE };

    struct StaticMeshTraits {
      StaticMeshTraits(Shape shape);
      StaticMeshTraits(const std::vector<float> &vertices, const std::vector<unsigned int> &indices)
          : vertices(vertices), indices(indices) {};

      std::vector<unsigned int> indices;
      std::vector<float> vertices;

      ShaderID shader_id = "default";
      MaterialID material_id = "default";
      Transform transform;
    };

    class StaticMesh {
    public:
      // Constructor is only public for in-place construction with
      // standard library types
      StaticMesh(const StaticMeshTraits &traits);
      StaticMesh(StaticMesh &&other)
          : vao(other.vao), indice_count(other.indice_count), transform(std::move(other.transform)),
            material_id(other.material_id) {
        other.vao = 0;
      }
      ~StaticMesh();

    protected:
      friend class Renderer;

      StaticMesh &operator=(StaticMesh &&other) {
        vao = other.vao;
        other.vao = 0;
        indice_count = other.indice_count;
        material_id = other.material_id;
        transform = std::move(other.transform);
        return *this;
      }
      StaticMesh(const StaticMesh &other) = delete;
      StaticMesh &operator=(const StaticMesh &other) = delete;

      MaterialID material_id = "default";

      Transform transform;

      void Draw() const;

      unsigned int vao = 0;
      unsigned int indice_count = 0;
      ShaderID shader_id = "default";
    };

  } // namespace Graphics
} // namespace Chemical
