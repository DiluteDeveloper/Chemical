#pragma once

#include <string>
#include <vector>

namespace Chemical {

  using ObjectID = std::string;

  namespace Graphics {

    // Used in StaticMeshTraits to choose the shape of the mesh
    enum class Shape { SQUARE, TRIANGLE };

    // Information to be used to generate a static mesh
    struct StaticMeshTraits {
      StaticMeshTraits(Shape shape = Shape::SQUARE, const ObjectID &transform_id = "default",
                       const ObjectID &material_id = "default");

      StaticMeshTraits(const ObjectID &transform_id = "default", const ObjectID &material_id = "default");
      std::vector<unsigned int> indices;
      std::vector<float> vertices;

      ObjectID material_id = "default";
      ObjectID transform_id = "default";
    };

    // Represents an OpenGL vertex array object and its functionality
    // Deleted copy ctor & operator, custom move ctor and operator
    class StaticMesh {
    public:
      // Generates the OpenGL vertex array object
      StaticMesh(const StaticMeshTraits &traits);
      // Deletes the OpenGL vertex array object
      ~StaticMesh();

      StaticMesh(StaticMesh &&other)
          : vao(other.vao), idx_count(other.idx_count), transform_id(other.transform_id),
            material_id(other.material_id) {
        other.vao = 0;
      }
      StaticMesh &operator=(StaticMesh &&other) {
        vao = other.vao;
        other.vao = 0;
        idx_count = other.idx_count;
        material_id = other.material_id;
        transform_id = other.transform_id;
        return *this;
      }

    protected:
      // All functionality that only the renderer/s should have access to
      friend class Renderer;

      StaticMesh(const StaticMesh &other) = delete;
      StaticMesh &operator=(const StaticMesh &other) = delete;

      void Draw() const;

      ObjectID material_id = "default";
      ObjectID transform_id = "default";

      // Not intended to be accessed by renderer/s
      unsigned int vao = 0;

      // Not intended to be accessed by renderer/s
      unsigned int idx_count = 0;
    };

  } // namespace Graphics
} // namespace Chemical
