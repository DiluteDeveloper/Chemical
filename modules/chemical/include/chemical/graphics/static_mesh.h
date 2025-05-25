#pragma once

#include "chemical/graphics/static_material.h"
#include "shader.h"

#include <vector>
namespace Chemical {

  namespace Graphics {

    class StaticMesh;

    enum class Shape { SQUARE, TRIANGLE };

    struct StaticMeshTraits {
      StaticMeshTraits(const std::vector<float> &vertices, const std::vector<unsigned int> &indices,
                       const ShaderID &shader_id = "default", const MaterialID &material_id = "default");
      StaticMeshTraits(Shape shape, const ShaderID &shader_id = "default",
                       const MaterialID &material_id = "default");

      std::vector<unsigned int> indices;
      std::vector<float> vertices;

      ShaderID shader_id = "default";
      MaterialID material_id = "default";
    };

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

      // temporarily publicised
      MaterialID material_id = "default";

    private:
      friend class Renderer;
      void Draw() const;

      unsigned int vao = 0;
      unsigned int indice_count = 0;
      ShaderID shader_id = 0;
    };

  } // namespace Graphics
} // namespace Chemical
