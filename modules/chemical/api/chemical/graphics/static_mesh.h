#pragma once

#include "shader.h"

#include <vector>
namespace Chemical {

  namespace Graphics {

    class StaticMesh;

    enum class Shape { SQUARE, TRIANGLE };

    struct StaticMeshTraits {
      StaticMeshTraits(const std::vector<float> &vertices, const std::vector<unsigned int> &indices,
                       ShaderID shader_id = 0);
      StaticMeshTraits(Shape shape, ShaderID shader_id = 0);

      std::vector<unsigned int> indices;
      std::vector<float> vertices;

      ShaderID shader_id = 0;
    };

    using StaticMeshID = int;
    class StaticMesh {
    public:
      StaticMesh(const StaticMeshTraits &traits);

      ~StaticMesh();
      StaticMesh(StaticMesh &&other) : vao(other.vao), indice_count(other.indice_count) { other.vao = 0; }
      StaticMesh &operator=(StaticMesh &&other) {
        vao = other.vao;
        other.vao = 0;
        indice_count = other.indice_count;
        return *this;
      }
      StaticMesh(const StaticMesh &other) = delete;
      StaticMesh &operator=(const StaticMesh &other) = delete;

    private:
      friend class Renderer;
      void Draw() const;

      unsigned int vao = 0;
      unsigned int indice_count = 0;
      ShaderID shader_id = 0;
    };

  } // namespace Graphics
} // namespace Chemical
