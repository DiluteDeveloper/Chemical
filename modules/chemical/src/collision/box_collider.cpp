#include "box_collider.hpp"

#include "rendering/cube.hpp"

namespace Chemical {

  BoxCollider3D::BoxCollider3D(float scale, const glm::vec3 &position)
      : position(position) {
    extents[0][0] = -(scale / 2);
    extents[0][1] = (scale / 2);
    extents[1][0] = -(scale / 2);
    extents[1][1] = (scale / 2);
    extents[2][0] = -(scale / 2);
    extents[2][1] = (scale / 2);
  }
  BoxCollider3D::BoxCollider3D(const glm::vec3 &scale,
                               const glm::vec3 &position)
      : position(position) {
    extents[0][0] = -(scale.x / 2);
    extents[0][1] = (scale.x / 2);
    extents[1][0] = -(scale.y / 2);
    extents[1][1] = (scale.y / 2);
    extents[2][0] = -(scale.z / 2);
    extents[2][1] = (scale.z / 2);
  }
  Mesh BoxCollider3D::AsMesh() {
    Mesh mesh;
    mesh.vertices = {
        /* clang-format off */
			Vertex(glm::vec3(extents[0][0], extents[1][0], extents[2][0]),
        glm::vec3(0.0f, -1.0f, 0.0f)), // 0 | BOTTOM BACK LEFT | BOTTOM NORMAL

      Vertex(glm::vec3(extents[0][0], extents[1][0], extents[2][0]),
        glm::vec3(0.0f, 0.0f, -1.0f)), // 1 | BOTTOM BACK LEFT | BACK NORMAL

      Vertex(glm::vec3(extents[0][0], extents[1][0], extents[2][0]),
        glm::vec3(-1.0f, 0.0f, 0.0f)), // 2 | BOTTOM BACK LEFT | LEFT NORMAL

      Vertex(glm::vec3(extents[0][0], extents[1][0], extents[2][1]),  
        glm::vec3(0.0f, -1.0f, 0.0f)), // 3 | BOTTOM FRONT LEFT | BOTTOM NORMAL

      Vertex(glm::vec3(extents[0][0], extents[1][0], extents[2][1]),  
        glm::vec3(0.0f, 0.0f, 1.0f)), // 4 | BOTTOM FRONT LEFT | FRONT NORMAL

      Vertex(glm::vec3(extents[0][0], extents[1][0], extents[2][1]),  
        glm::vec3(-1.0f, 0.0f, 0.0f)), // 5 | BOTTOM FRONT LEFT | LEFT NORMAL

      Vertex(glm::vec3(extents[0][1], extents[1][0], extents[2][1]),  
        glm::vec3(0.0f, -1.0f, 0.0f)), // 6 | BOTTOM FRONT RIGHT | BOTTOM NORMAL

      Vertex(glm::vec3(extents[0][1], extents[1][0], extents[2][1]),  
        glm::vec3(0.0f, 0.0f, 1.0f)), // 7 | BOTTOM FRONT RIGHT | FRONT NORMAL

      Vertex(glm::vec3(extents[0][1], extents[1][0], extents[2][1]),  
        glm::vec3(1.0f, 0.0f, 0.0f)), // 8 | BOTTOM FRONT RIGHT | RIGHT NORMAL

      Vertex(glm::vec3(extents[0][1], extents[1][0], extents[2][0]), 
        glm::vec3(0.0f, -1.0f, 0.0f)), // 9 | BOTTOM BACK RIGHT | BOTTOM NORMAL

      Vertex(glm::vec3(extents[0][1], extents[1][0], extents[2][0]), 
        glm::vec3(0.0f, 0.0f, -1.0f)), // 10 | BOTTOM BACK RIGHT | BACK NORMAL

      Vertex(glm::vec3(extents[0][1], extents[1][0], extents[2][0]), 
        glm::vec3(1.0f, 0.0f, 0.0f)), // 11 | BOTTOM BACK RIGHT | RIGHT NORMAL

      Vertex(glm::vec3(extents[0][0], extents[1][1], extents[2][0]), 
        glm::vec3(0.0f, 1.0f, 0.0f)), // 12 | TOP BACK LEFT | TOP NORMAL

      Vertex(glm::vec3(extents[0][0], extents[1][1], extents[2][0]), 
        glm::vec3(0.0f, 0.0f, -1.0f)), // 13 | TOP BACK LEFT | BACK NORMAL

      Vertex(glm::vec3(extents[0][0], extents[1][1], extents[2][0]), 
        glm::vec3(-1.0f, 0.0f, 0.0f)), // 14 | TOP BACK LEFT | LEFT NORMAL

      Vertex(glm::vec3(extents[0][0], extents[1][1], extents[2][1]),  
        glm::vec3(0.0f, 1.0f, 0.0f)), // 15 | TOP FRONT LEFT | TOP NORMAL

      Vertex(glm::vec3(extents[0][0], extents[1][1], extents[2][1]),  
        glm::vec3(0.0f, 0.0f, 1.0f)), // 16 | TOP FRONT LEFT | FRONT NORMAL

      Vertex(glm::vec3(extents[0][0], extents[1][1], extents[2][1]),  
        glm::vec3(-1.0f, 0.0f, 0.0f)), // 17 | TOP FRONT LEFT | LEFT NORMAL

      Vertex(glm::vec3(extents[0][1], extents[1][1], extents[2][1]),  
        glm::vec3(0.0f, 1.0f, 0.0f)), // 18 | TOP FRONT RIGHT | TOP NORMAL

      Vertex(glm::vec3(extents[0][1], extents[1][1], extents[2][1]),  
        glm::vec3(0.0f, 0.0f, 1.0f)), // 19 | TOP FRONT RIGHT | FRONT NORMAL

      Vertex(glm::vec3(extents[0][1], extents[1][1], extents[2][1]),  
        glm::vec3(1.0f, 0.0f, 0.0f)), // 20 | TOP FRONT RIGHT | RIGHT NORMAL

      Vertex(glm::vec3(extents[0][1], extents[1][1], extents[2][0]),
        glm::vec3(0.0f, 1.0f, 0.0f)), // 21 | TOP BACK RIGHT | TOP NORMAL

      Vertex(glm::vec3(extents[0][1], extents[1][1], extents[2][0]),  
        glm::vec3(0.0f, 0.0f, -1.0f)), // 22 | TOP BACK RIGHT | BACK NORMAL

      Vertex(glm::vec3(extents[0][1], extents[1][1], extents[2][0]),  
        glm::vec3(1.0f, 0.0f, 0.0f)) // 23 | TOP BACK RIGHT | RIGHT NORMAL

        /* clang-format on */
    };
    mesh.indices = GetCubeIndices();
    return mesh;
  }
  bool BoxCollider3D::IsCollidingWith(const BoxCollider3D &other) const {
    // Check if inside X
    if (extents[0][0] + position.x > other.extents[0][1] + other.position.x ||
        extents[0][1] + position.x < other.extents[0][0] + other.position.x)
      return false;
    if (extents[1][0] + position.y > other.extents[1][1] + other.position.y ||
        extents[1][1] + position.y < other.extents[1][0] + other.position.y)
      return false;
    if (extents[2][0] + position.z > other.extents[2][1] + other.position.z ||
        extents[2][1] + position.z < other.extents[2][0] + other.position.z)
      return false;
    return true;
  }
} // namespace Chemical
