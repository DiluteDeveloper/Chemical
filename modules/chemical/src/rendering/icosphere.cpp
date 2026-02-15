#include "icosphere.hpp"
#include "glm/geometric.hpp"
#include "spdlog/spdlog.h"

namespace Chemical {

  // Resolution represents how many triangles per horizontal loop
  Mesh GenerateIcosphereFlatNormals(unsigned int resolution) {
    Mesh mesh;

    // angle of a triangle section of the circle in radians (divide by 2 for
    // half of a circle for vertical)
    float res_radians = 1 / (resolution / (3.1415f * 2.0f));

    for (int v = 0; v < resolution; v++) {
      // Iterate through the vertical levels of the icosphere
      for (int h = 0; h < resolution; h++) {
        // Iterate through each vertex on the current Y plane of the sphere

        float h_offset = v % 2 ? (res_radians / 2.0f) : 0;

        float prev_angle = ((h - 1) * res_radians) + h_offset;
        float cur_angle = (h * res_radians) + h_offset;
        float next_angle = ((h + 1) * res_radians) + h_offset;

        // resolution in radians is half because vertical is only half of a
        // circle
        float cur_angle_vert = v * (res_radians / 2.0f);
        float next_angle_vert = (v + 1) * (res_radians / 2.0f);

        glm::vec3 v0_pos =
            glm::vec3(cos(cur_angle) * sin(cur_angle_vert), cos(cur_angle_vert),
                      sin(cur_angle) * sin(cur_angle_vert));

        glm::vec3 v1_pos = glm::vec3(
            cos(next_angle - (res_radians / 2.0f)) * sin(next_angle_vert),
            cos(next_angle_vert),
            sin(next_angle - (res_radians / 2.0f)) * sin(next_angle_vert));

        glm::vec3 v2_pos = glm::vec3(cos(next_angle) * sin(cur_angle_vert),
                                     cos(cur_angle_vert),
                                     sin(next_angle) * sin(cur_angle_vert));

        // The upside down triangle from previous iteration
        glm::vec3 v_neg_pos = glm::vec3(
            cos(prev_angle + (res_radians / 2.0f)) * sin(next_angle_vert),
            cos(next_angle_vert),
            sin(prev_angle + (res_radians / 2.0f)) * sin(next_angle_vert));

        if (v != 0) {

          glm::vec3 tri_pos_normal = glm::normalize(
              glm::vec3(glm::cross(v1_pos - v0_pos, v2_pos - v0_pos)));
          mesh.vertices.emplace_back(v0_pos, tri_pos_normal);
          mesh.vertices.emplace_back(v1_pos, tri_pos_normal);
          mesh.vertices.emplace_back(v2_pos, tri_pos_normal);
        }

        // SPDLOG_INFO("x: {}, y: {}, z: {}", v0_pos.x, v0_pos.y, v0_pos.z);
        // SPDLOG_INFO("x: {}, y: {}, z: {}", v1_pos.x, v1_pos.y, v1_pos.z);
        // SPDLOG_INFO("x: {}, y: {}, z: {}", v2_pos.x, v2_pos.y, v2_pos.z);

        if (v != resolution - 1) {
          glm::vec3 tri_neg_normal = glm::normalize(
              glm::vec3(glm::cross(v_neg_pos - v0_pos, v1_pos - v0_pos)));
          mesh.vertices.emplace_back(v0_pos, tri_neg_normal);
          mesh.vertices.emplace_back(v_neg_pos, tri_neg_normal);
          mesh.vertices.emplace_back(v1_pos, tri_neg_normal);
        }
      }
    }

    // for (Vertex &v : mesh.vertices) {
    //   SPDLOG_INFO("{}, {}, {}", v.position.x, v.position.y, v.position.z);
    // }
    return mesh;
  }

  // Resolution represents how many triangles per horizontal loop
  Mesh GenerateIcosphereSmoothNormals(unsigned int resolution) {
    Mesh mesh;

    // angle of a triangle section of the circle in radians (divide by 2 for
    // half of a circle for vertical)
    float res_radians = 1 / (resolution / (3.1415f * 2.0f));

    mesh.vertices.emplace_back(glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    // Iterate through vertices
    for (int v = 1; v < resolution - 1; v++) {
      // Iterate through the vertical levels of the icosphere
      for (int h = 0; h < resolution; h++) {
        // Iterate through each vertex on the current Y plane of the sphere

        float h_offset = v % 2 ? (res_radians / 2.0f) : 0;

        // float prev_angle = ((h - 1) * res_radians) + h_offset;
        float cur_angle = (h * res_radians) + h_offset;
        // float next_angle = ((h + 1) * res_radians) + h_offset;

        // resolution in radians is half because vertical is only half of a
        // circle
        float cur_angle_vert = v * (res_radians / 2.0f);
        float next_angle_vert = (v + 1) * (res_radians / 2.0f);

        glm::vec3 v0_pos =
            glm::vec3(cos(cur_angle) * sin(cur_angle_vert), cos(cur_angle_vert),
                      sin(cur_angle) * sin(cur_angle_vert));

        mesh.vertices.emplace_back(v0_pos);
      }
    }
    mesh.vertices.emplace_back(glm::vec3(0, -1, 0), glm::vec3(0, -1, 0));

    std::vector<std::pair<glm::vec3, glm::vec3>>
        tri_normals; // each index in this array represents a vertex (excluding
                     // v=0 vertex and v=resolution-1 vertex);
                     // Upside down triangle | Right-side up triangle
    // Iterate through indices
    for (int v = 0; v < resolution; v++) {
      // Iterate through the vertical levels of the icosphere
      for (int h = 0; h < resolution; h++) {
        // Iterate through each vertex on the current Y plane of the sphere

        // on v=0, current_vertex is always 0; only 1 vertex on top and bottomjj
        int current_vertex =
            ((h + (v * resolution)) - (resolution - 1)) * glm::sign(v);

        // Not used for v=0
        int next_hori_vertex =
            (((h + 1) % (resolution)) + (v * resolution)) - (resolution - 1);

        // always equals the last vertex when v=last y plane
        int next_vert_vertex =
            std::min((((h - 1) + (!glm::sign(h) * resolution)) +
                      ((v + 1) * resolution)) -
                         (resolution - 1),
                     (int)pow(resolution, 2) - (resolution - 1));

        // not used for v=resolution-1
        int next_hori_next_vert_vertex =
            (h + ((v + 1) * resolution)) - (resolution - 1);

        // Only used for normals -------

        int prev_vert_vertex =
            std::max((int)((h + ((v - 1) * resolution)) - (resolution - 1)), 0);

        int prev_vert_next_hori_vertex =
            std::max((int)((((h + 1) % (resolution)) + ((v - 1) * resolution)) -
                           (resolution - 1)),
                     0);

        int prev_hori_vertex =
            std::max((int)((((h - 1) + (!glm::sign(h) * resolution)) +
                            (v * resolution)) -
                           (resolution - 1)),
                     0);
        // Only used for normals -------

        SPDLOG_INFO("v: {}", v);
        SPDLOG_INFO("h: {}", h);
        SPDLOG_INFO("current: {}", current_vertex);
        SPDLOG_INFO("prev vert: {}", prev_vert_vertex);
        SPDLOG_INFO("prev vert next hori: {}", prev_vert_next_hori_vertex);
        SPDLOG_INFO("prev hori: {}", prev_hori_vertex);
        if (v != 0 && v != resolution - 1) {

          tri_normals.emplace_back(std::make_pair(
              (glm::vec3(glm::normalize(glm::cross(
                  mesh.vertices[current_vertex].position -
                      mesh.vertices[next_vert_vertex].position,
                  mesh.vertices[next_hori_next_vert_vertex].position -
                      mesh.vertices[next_vert_vertex].position)))),

              glm::vec3(glm::normalize(glm::cross(
                  mesh.vertices[next_hori_vertex].position -
                      mesh.vertices[current_vertex].position,
                  mesh.vertices[current_vertex].position -
                      mesh.vertices[next_hori_next_vert_vertex].position)))));
        }
        if (v != 0) { // Dont do upside down triangles for top section

          mesh.indices.emplace_back(next_vert_vertex);
          mesh.indices.emplace_back(current_vertex);
          mesh.indices.emplace_back(next_hori_next_vert_vertex);
        }
        if (v != resolution -
                     1) { // Dont do right-side up triangles for bottom section
          mesh.indices.emplace_back(current_vertex);
          mesh.indices.emplace_back(next_hori_vertex);
          mesh.indices.emplace_back(next_hori_next_vert_vertex);
        }
      }
    }

    // Iterate through vertices again to setup smooth normals
    for (int v = 1; v < resolution - 1; v++) {
      // Iterate through the vertical levels of the icosphere
      for (int h = 0; h < resolution; h++) {
        // Iterate through each vertex on the current Y plane of the sphere

        glm::vec3 tri_normal_1 = glm::vec3(0, 1, 0);
        glm::vec3 tri_normal_2 = glm::vec3;
        glm::vec3 tri_normal_3;
        if (v == 1) {

          glm::vec3 tri_normal_1 =
              tri_normals[(h + ((v - 1) * resolution)) * 2].second;
          glm::vec3 tri_normal_2 =
              tri_normals[(h + ((v - 1) * resolution)) * 2].first;
        }
      }
    }
    return mesh;
  }
} // namespace Chemical
