#include "box_collider.hpp"

#include "rendering/cube.hpp"

namespace Chemical {

  BoxCollider3D::BoxCollider3D(float extents, const glm::vec3 &position)
      : position(position), extents(glm::vec3(extents)) {}
  BoxCollider3D::BoxCollider3D(const glm::vec3 &extents,
                               const glm::vec3 &position)
      : position(position), extents(extents) {}
  Mesh BoxCollider3D::AsMesh() {
    Mesh mesh;
    mesh.vertices = {
        /* clang-format off */
			Vertex(glm::vec3(-extents.x, -extents.y, -extents.z),
        glm::vec3(0.0f, -1.0f, 0.0f)), // 0 | BOTTOM BACK LEFT | BOTTOM NORMAL

      Vertex(glm::vec3(-extents.x, -extents.y, -extents.z),
        glm::vec3(0.0f, 0.0f, -1.0f)), // 1 | BOTTOM BACK LEFT | BACK NORMAL

      Vertex(glm::vec3(-extents.x, -extents.y, -extents.z),
        glm::vec3(-1.0f, 0.0f, 0.0f)), // 2 | BOTTOM BACK LEFT | LEFT NORMAL

      Vertex(glm::vec3(-extents.x, -extents.y, extents.z),  
        glm::vec3(0.0f, -1.0f, 0.0f)), // 3 | BOTTOM FRONT LEFT | BOTTOM NORMAL

      Vertex(glm::vec3(-extents.x, -extents.y, extents.z),  
        glm::vec3(0.0f, 0.0f, 1.0f)), // 4 | BOTTOM FRONT LEFT | FRONT NORMAL

      Vertex(glm::vec3(-extents.x, -extents.y, extents.z),  
        glm::vec3(-1.0f, 0.0f, 0.0f)), // 5 | BOTTOM FRONT LEFT | LEFT NORMAL

      Vertex(glm::vec3(extents.x, -extents.y, extents.z),  
        glm::vec3(0.0f, -1.0f, 0.0f)), // 6 | BOTTOM FRONT RIGHT | BOTTOM NORMAL

      Vertex(glm::vec3(extents.x, -extents.y, extents.z),  
        glm::vec3(0.0f, 0.0f, 1.0f)), // 7 | BOTTOM FRONT RIGHT | FRONT NORMAL

      Vertex(glm::vec3(extents.x, -extents.y, extents.z),  
        glm::vec3(1.0f, 0.0f, 0.0f)), // 8 | BOTTOM FRONT RIGHT | RIGHT NORMAL

      Vertex(glm::vec3(extents.x, -extents.y, -extents.z), 
        glm::vec3(0.0f, -1.0f, 0.0f)), // 9 | BOTTOM BACK RIGHT | BOTTOM NORMAL

      Vertex(glm::vec3(extents.x, -extents.y, -extents.z), 
        glm::vec3(0.0f, 0.0f, -1.0f)), // 10 | BOTTOM BACK RIGHT | BACK NORMAL

      Vertex(glm::vec3(extents.x, -extents.y, -extents.z), 
        glm::vec3(1.0f, 0.0f, 0.0f)), // 11 | BOTTOM BACK RIGHT | RIGHT NORMAL

      Vertex(glm::vec3(-extents.x, extents.y, -extents.z), 
        glm::vec3(0.0f, 1.0f, 0.0f)), // 12 | TOP BACK LEFT | TOP NORMAL

      Vertex(glm::vec3(-extents.x, extents.y, -extents.z), 
        glm::vec3(0.0f, 0.0f, -1.0f)), // 13 | TOP BACK LEFT | BACK NORMAL

      Vertex(glm::vec3(-extents.x, extents.y, -extents.z), 
        glm::vec3(-1.0f, 0.0f, 0.0f)), // 14 | TOP BACK LEFT | LEFT NORMAL

      Vertex(glm::vec3(-extents.x, extents.y, extents.z),  
        glm::vec3(0.0f, 1.0f, 0.0f)), // 15 | TOP FRONT LEFT | TOP NORMAL

      Vertex(glm::vec3(-extents.x, extents.y, extents.z),  
        glm::vec3(0.0f, 0.0f, 1.0f)), // 16 | TOP FRONT LEFT | FRONT NORMAL

      Vertex(glm::vec3(-extents.x, extents.y, extents.z),  
        glm::vec3(-1.0f, 0.0f, 0.0f)), // 17 | TOP FRONT LEFT | LEFT NORMAL

      Vertex(glm::vec3(extents.x, extents.y, extents.z),  
        glm::vec3(0.0f, 1.0f, 0.0f)), // 18 | TOP FRONT RIGHT | TOP NORMAL

      Vertex(glm::vec3(extents.x, extents.y, extents.z),  
        glm::vec3(0.0f, 0.0f, 1.0f)), // 19 | TOP FRONT RIGHT | FRONT NORMAL

      Vertex(glm::vec3(extents.x, extents.y, extents.z),  
        glm::vec3(1.0f, 0.0f, 0.0f)), // 20 | TOP FRONT RIGHT | RIGHT NORMAL

      Vertex(glm::vec3(extents.x, extents.y, -extents.z),
        glm::vec3(0.0f, 1.0f, 0.0f)), // 21 | TOP BACK RIGHT | TOP NORMAL

      Vertex(glm::vec3(extents.x, extents.y, -extents.z),  
        glm::vec3(0.0f, 0.0f, -1.0f)), // 22 | TOP BACK RIGHT | BACK NORMAL

      Vertex(glm::vec3(extents.x, extents.y, -extents.z),  
        glm::vec3(1.0f, 0.0f, 0.0f)) // 23 | TOP BACK RIGHT | RIGHT NORMAL

        /* clang-format on */
    };
    mesh.indices = GetCubeIndices();
    return mesh;
  }
  bool BoxCollider3D::IsCollidingWith(const BoxCollider3D &other) const {
    // Check if inside X

    float global_xp = extents.x + position.x;
    float global_yp = extents.y + position.y;
    float global_zp = extents.z + position.y;
    float global_xn = -extents.x + position.x;
    float global_yn = -extents.y + position.y;
    float global_zn = -extents.z + position.y;
    float other_global_xp = other.extents.x + other.position.x;
    float other_global_yp = other.extents.y + other.position.y;
    float other_global_zp = other.extents.z + other.position.y;
    float other_global_xn = -other.extents.x + other.position.x;
    float other_global_yn = -other.extents.y + other.position.y;
    float other_global_zn = -other.extents.z + other.position.y;
    if (global_xn < other_global_xp || global_xp > other_global_xn)
      return true;

    if (global_yn < other_global_yp || global_yp > other_global_yn)
      return true;

    if (global_zn < other_global_zp || global_zp > other_global_zn)
      return true;

    return false;
  }
} // namespace Chemical
