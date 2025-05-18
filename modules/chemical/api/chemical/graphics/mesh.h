#pragma once

#include <vector>

namespace Chemical {

  namespace Graphics {

    enum class Shape2D { SQUARE, TRIANGLE };
    class StaticMesh2D {
    public:
      ~StaticMesh2D();

      void Draw();

    private:
      const unsigned int VAO = 0;
      const unsigned int indices = 0;

      StaticMesh2D(unsigned int VAO, unsigned int indices)
          : VAO(VAO), indices(indices) {}

      friend StaticMesh2D
      CreateStaticMesh2D(const std::vector<float> vertices,
                         const std::vector<unsigned int> indices);
      friend StaticMesh2D CreateStaticMesh2D(Shape2D shape);
    };

    StaticMesh2D CreateStaticMesh2D(const std::vector<float> vertices,
                                    const std::vector<unsigned int> indices);
    StaticMesh2D CreateStaticMesh2D(Shape2D shape);

  } // namespace Graphics
} // namespace Chemical
