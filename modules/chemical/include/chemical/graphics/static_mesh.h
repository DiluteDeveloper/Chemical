#pragma once

#include <string>
#include <vector>

namespace Chemical {

  using ObjectID = std::string;

  namespace Graphics {

    class StaticMesh;

    enum class Shape { SQUARE, TRIANGLE };

    struct StaticMeshTraits {
      StaticMeshTraits(Shape shape);
      StaticMeshTraits(const std::vector<float> &vertices, const std::vector<unsigned int> &indices)
          : vertices(vertices), indices(indices) {};

      std::vector<unsigned int> indices;
      std::vector<float> vertices;

      ObjectID material_id = "default";
      ObjectID shader_id = "default";
      ObjectID transform_id = "default";
    };

    class StaticMesh {
    public:
      // Constructor is only public for in-place construction with
      // standard library types
      StaticMesh(const StaticMeshTraits &traits);
      StaticMesh(StaticMesh &&other)
          : vao(other.vao), indice_count(other.indice_count), transform_id(other.transform_id),
            material_id(other.material_id) {
        other.vao = 0;
      }
      ~StaticMesh();

    protected:
      friend class Renderer;
      friend class SceneRenderer;

      StaticMesh &operator=(StaticMesh &&other) {
        vao = other.vao;
        other.vao = 0;
        indice_count = other.indice_count;
        material_id = other.material_id;
        transform_id = other.transform_id;
        return *this;
      }
      StaticMesh(const StaticMesh &other) = delete;
      StaticMesh &operator=(const StaticMesh &other) = delete;

      void Draw() const;

      unsigned int vao = 0;
      unsigned int indice_count = 0;
      ObjectID material_id = "default";
      ObjectID transform_id = "default";
    };

  } // namespace Graphics
} // namespace Chemical
