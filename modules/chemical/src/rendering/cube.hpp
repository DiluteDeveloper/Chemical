#include <vector>

namespace Chemical {

  inline constexpr std::vector<float> GetCubeVertices() {
    static std::vector<float> vertices = {
        -0.5f, -0.5f, -0.5f, // 0 - BOTTOM BACK LEFT
        -0.5f, -0.5f, 0.5f,  // 1 - BOTTOM FRONT LEFT
        0.5f,  -0.5f, 0.5f,  // 2 - BOTTOM FRONT RIGHT
        0.5f,  -0.5f, -0.5f, // 3 - BOTTOM BACK RIGHT
        -0.5f, 0.5f,  -0.5f, // 4 - TOP BACK LEFT
        -0.5f, 0.5f,  0.5f,  // 5 - TOP FRONT LEFT
        0.5f,  0.5f,  0.5f,  // 6 - TOP FRONT RIGHT
        0.5f,  0.5f,  -0.5f  // 7 - TOP BACK RIGHT
    };
    return vertices;
  }
  inline constexpr std::vector<unsigned int> GetCubeIndices() {
    static std::vector<unsigned int> indices = {
        2, 1, 0, 0, 3, 2, // BOTTOM FACE
        4, 5, 6, 6, 7, 4, // TOP FACE
        0, 4, 7, 7, 3, 0, // BACK FACE
        2, 6, 5, 5, 1, 2, // FRONT FACE
        1, 5, 4, 4, 0, 1, // LEFT FACE
        2, 6, 7, 7, 3, 2  // RIGHT FACE // WIP
    };
    return indices;
  }
} // namespace Chemical
