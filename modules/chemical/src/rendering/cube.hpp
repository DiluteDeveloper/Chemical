#include <vector>

namespace Chemical {

  inline constexpr std::vector<float> GetCubeVertices() {
    static std::vector<float> vertices = {
        /* clang-format off */
        -0.5f, -0.5f, -0.5f,
        0.0f,  -1.0f, 0.0f, // 0 | BOTTOM BACK LEFT | BOTTOM NORMAL

        -0.5f, -0.5f, -0.5f,
        0.0f,  0.0f, -1.0f, // 1 | BOTTOM BACK LEFT | BACK NORMAL

        -0.5f, -0.5f, -0.5f,
        -1.0f,  0.0f, 0.0f, // 2 | BOTTOM BACK LEFT | LEFT NORMAL

        -0.5f, -0.5f, 0.5f,  
        0.0f,  -1.0f, 0.0f, // 3 | BOTTOM FRONT LEFT | BOTTOM NORMAL

        -0.5f, -0.5f, 0.5f,  
        0.0f,  0.0f, 1.0f, // 4 | BOTTOM FRONT LEFT | FRONT NORMAL

        -0.5f, -0.5f, 0.5f,  
        -1.0f,  0.0f, 0.0f, // 5 | BOTTOM FRONT LEFT | LEFT NORMAL

        0.5f,  -0.5f, 0.5f,  
        0.0f,  -1.0f, 0.0f, // 6 | BOTTOM FRONT RIGHT | BOTTOM NORMAL

        0.5f,  -0.5f, 0.5f,  
        0.0f,  0.0f, 1.0f, // 7 | BOTTOM FRONT RIGHT | FRONT NORMAL

        0.5f,  -0.5f, 0.5f,  
        1.0f,  0.0f, 0.0f, // 8 | BOTTOM FRONT RIGHT | RIGHT NORMAL

        0.5f,  -0.5f, -0.5f, 
        0.0f,  -1.0f, 0.0f, // 9 | BOTTOM BACK RIGHT | BOTTOM NORMAL

        0.5f,  -0.5f, -0.5f, 
        0.0f,  0.0f, -1.0f, // 10 | BOTTOM BACK RIGHT | BACK NORMAL

        0.5f,  -0.5f, -0.5f, 
        1.0f,  0.0f, 0.0f, // 11 | BOTTOM BACK RIGHT | RIGHT NORMAL

        -0.5f, 0.5f,  -0.5f, 
        0.0f,  1.0f, 0.0f, // 12 | TOP BACK LEFT | TOP NORMAL

        -0.5f, 0.5f,  -0.5f, 
        0.0f,  0.0f, -1.0f, // 13 | TOP BACK LEFT | BACK NORMAL

        -0.5f, 0.5f,  -0.5f, 
        -1.0f,  0.0f, 0.0f, // 14 | TOP BACK LEFT | LEFT NORMAL

        -0.5f, 0.5f,  0.5f,  
        0.0f,  1.0f, 0.0f, // 15 | TOP FRONT LEFT | TOP NORMAL

        -0.5f, 0.5f,  0.5f,  
        0.0f,  0.0f, 1.0f, // 16 | TOP FRONT LEFT | FRONT NORMAL

        -0.5f, 0.5f,  0.5f,  
        -1.0f,  0.0f, 0.0f, // 17 | TOP FRONT LEFT | LEFT NORMAL

        0.5f,  0.5f,  0.5f,  
        0.0f,  1.0f, 0.0f, // 18 | TOP FRONT RIGHT | TOP NORMAL

        0.5f,  0.5f,  0.5f,  
        0.0f,  0.0f, 1.0f, // 19 | TOP FRONT RIGHT | FRONT NORMAL

        0.5f,  0.5f,  0.5f,  
        1.0f,  0.0f, 0.0f, // 20 | TOP FRONT RIGHT | RIGHT NORMAL

        0.5f,  0.5f,  -0.5f,
        0.0f,  1.0f, 0.0f, // 21 | TOP BACK RIGHT | TOP NORMAL

        0.5f,  0.5f,  -0.5f,  
        0.0f,  0.0f, -1.0f, // 22 | TOP BACK RIGHT | BACK NORMAL

        0.5f,  0.5f,  -0.5f,  
        1.0f,  0.0f, 0.0f // 23 | TOP BACK RIGHT | RIGHT NORMAL

        /* clang-format on */
    };
    return vertices;
  }
  inline constexpr std::vector<unsigned int> GetCubeIndices() {
    static std::vector<unsigned int> indices = {
        6,  3,  0,  0,  9,  6,  // BOTTOM FACE
        12, 15, 18, 18, 21, 12, // TOP FACE
        1,  13, 22, 22, 10, 1,  // BACK FACE
        7,  19, 16, 16, 4,  7,  // FRONT FACE
        5,  17, 14, 14, 2,  5,  // LEFT FACE
        23, 20, 8,  8,  11, 23  // RIGHT FACE
    };
    return indices;
  }
} // namespace Chemical
