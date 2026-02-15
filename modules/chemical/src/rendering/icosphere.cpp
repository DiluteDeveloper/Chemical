#include "icosphere.hpp"
#include "glm/geometric.hpp"
#include "spdlog/spdlog.h"
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
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

        float h_offset = v * (res_radians / 2.0f);

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
    for (int v = 1; v < resolution; v++) {
      // Iterate through the vertical levels of the icosphere
      for (int h = 0; h < resolution; h++) {
        // Iterate through each vertex on the current Y plane of the sphere

        float h_offset = v * (res_radians / 2.0f);

        // float prev_angle = ((h - 1) * res_radians) + h_offset;
        float cur_angle = (h * res_radians) + h_offset;
        // float next_angle = ((h + 1) * res_radians) + h_offset;

        // resolution in radians is half because vertical is only half of a
        // circle
        float cur_angle_vert = v * (res_radians / 2.0f);

        glm::vec3 v0_pos =
            glm::vec3(cos(cur_angle) * sin(cur_angle_vert), cos(cur_angle_vert),
                      sin(cur_angle) * sin(cur_angle_vert));

        // SPDLOG_INFO("element: {} : {}, {}, {}", mesh.vertices.size(),
        // v0_pos.x,
        //             v0_pos.y, v0_pos.z);
        // if (v == 1 & h == 0)
        //   mesh.vertices.emplace_back(v0_pos, glm::vec3(1, 0, 0));
        // else
        mesh.vertices.emplace_back(v0_pos);
      }
    }
    mesh.vertices.emplace_back(glm::vec3(0, -1, 0), glm::vec3(0, -1, 0));

    // has resolution*2 elements
    // first triangle is the right side up second is upside down
    // Top and bottom have redundant 0,0,0 normals for the non-applicable
    // triangles
    std::vector<glm::fvec3> tri_normals;
    tri_normals.resize(glm::pow(resolution, 2) * 2);

    // first horizontal index of the 2nd to last layer of the triangle (actual
    // last layer is just 1 vert)
    int last_layer_idx = ((glm::pow(resolution, 2) - (resolution * 2)) + 1);

    // Generate face normals and indices
    for (int h = 0; h < resolution; h++) {

      // Generate first layer and last layer
      {
        // Positive vertical and positive horizontal index into vertices array
        int v1_h0_idx = h + 1;
        int v1_h1_idx = (v1_h0_idx % resolution) + 1;
        int v0_idx = 0;

        mesh.indices.emplace_back(v1_h1_idx);
        mesh.indices.emplace_back(v1_h0_idx);
        mesh.indices.emplace_back(v0_idx);

        tri_normals[h * 2] = glm::normalize(
            glm::cross(glm::vec3(mesh.vertices[v1_h0_idx].position -
                                 mesh.vertices[v1_h1_idx].position),
                       glm::vec3(mesh.vertices[v0_idx].position -
                                 mesh.vertices[v1_h1_idx].position)));

        // v2= last layer
        int v2_h0_idx = h + last_layer_idx;
        int v2_h1_idx = ((h + 1) % resolution) + last_layer_idx;

        // v3 = actual last vertex
        int v3_idx = mesh.vertices.size() - 1;

        mesh.indices.emplace_back(v3_idx);
        mesh.indices.emplace_back(v2_h0_idx);
        mesh.indices.emplace_back(v2_h1_idx);

        tri_normals[((tri_normals.size() - (resolution * 2)) + (h * 2)) + 1] =
            glm::normalize(
                glm::cross(glm::vec3(mesh.vertices[v2_h0_idx].position -
                                     mesh.vertices[v3_idx].position),
                           glm::vec3(mesh.vertices[v2_h1_idx].position -
                                     mesh.vertices[v3_idx].position)));
      }
    }
    // generate the middle layers
    for (int v = 0; v < resolution - 2; v++) {
      for (int h = 0; h < resolution; h++) {

        int v0_h0_idx = h + (v * resolution) + 1;
        int v0_h1_idx = ((h + 1) % resolution) + (v * resolution) + 1;
        int v1_h0_idx = h + ((v + 1) * resolution) + 1;
        int v1_hneg1_idx =
            (((h - 1) + resolution) % resolution) + ((v + 1) * resolution) + 1;

        // SPDLOG_INFO("v: {}, h: {}, v0_h0_idx: {}, v0_h1_idx: {}, v1_h0_idx: "
        //             "{}, v1_hneg1_idx: {}",
        //             v, h, v0_h0_idx, v0_h1_idx, v1_h0_idx, v1_hneg1_idx);
        //
        // SPDLOG_INFO("idx: {}", ((h + (v * resolution)) * 2) + resolution);
        // SPDLOG_INFO("idx 2: {}", ((h + (v * resolution)) * 2) + resolution +
        // 1);

        { // right side up triangle
          mesh.indices.emplace_back(v1_h0_idx);
          mesh.indices.emplace_back(v0_h0_idx);
          mesh.indices.emplace_back(v0_h1_idx);

          tri_normals[((h + (v * resolution)) * 2) + (resolution * 2)] =
              glm::normalize(
                  glm::cross(glm::vec3(mesh.vertices[v0_h0_idx].position -
                                       mesh.vertices[v1_h0_idx].position),
                             glm::vec3(mesh.vertices[v0_h1_idx].position -
                                       mesh.vertices[v1_h0_idx].position)));
        }

        { // upside down triangle
          mesh.indices.emplace_back(v1_h0_idx);
          mesh.indices.emplace_back(v1_hneg1_idx);
          mesh.indices.emplace_back(v0_h0_idx);

          tri_normals[((h + (v * resolution)) * 2) + (resolution * 2) + 1] =
              glm::normalize(
                  glm::cross(glm::vec3(mesh.vertices[v1_hneg1_idx].position -
                                       mesh.vertices[v1_h0_idx].position),
                             glm::vec3(mesh.vertices[v0_h0_idx].position -
                                       mesh.vertices[v1_h0_idx].position)));
        }
      }
    }

    // Iterate through vertices make averaged normals
    for (int v = 0; v < resolution - 1; v++) {
      // Iterate through the vertical levels of the icosphere
      for (int h = 0; h < resolution; h++) {
        // Iterate through each vertex on the current Y plane of the sphere

        // Vertices are on the NEXT layer from the triangles
        int cur_vtx_idx = h + (v * resolution) + 1;

        int v0_tri1_idx = ((h + (v * resolution)) * 2);
        int v0_tri2_idx = v0_tri1_idx + 1;
        int v1_tri3_idx = ((((h + 1) % resolution) + (v * resolution)) * 2);

        int v2_tri4_idx =
            (((((h - 1) + resolution) % resolution) + ((v + 1) * resolution)) *
             2) +
            1;

        int v3_tri5_idx = (h + ((v + 1) * resolution)) * 2;
        int v3_tri6_idx = ((h + ((v + 1) * resolution)) * 2) + 1;

        glm::fvec3 sum = glm::fvec3(0, 0, 0);

        sum += tri_normals[v0_tri1_idx];
        sum += tri_normals[v0_tri2_idx];
        sum += tri_normals[v1_tri3_idx];
        sum += tri_normals[v2_tri4_idx];
        sum += tri_normals[v3_tri5_idx];
        sum += tri_normals[v3_tri6_idx];

        // if (v0_tri1_idx > tri_normals.size() - 1)
        //   SPDLOG_ERROR("ERROR: {}", v0_tri1_idx);
        // if (v0_tri2_idx > tri_normals.size() - 1)
        //   SPDLOG_ERROR("ERROR: {}", v0_tri2_idx);
        // if (v1_tri3_idx > tri_normals.size() - 1)
        //   SPDLOG_ERROR("ERROR: {}", v1_tri3_idx);
        // if (v2_tri4_idx > tri_normals.size() - 1)
        //   SPDLOG_ERROR("ERROR: {}", v2_tri4_idx);
        // if (v3_tri5_idx > tri_normals.size() - 1)
        //   SPDLOG_ERROR("ERROR: {}", v3_tri5_idx);
        // if (v3_tri6_idx > tri_normals.size() - 1)
        //   SPDLOG_ERROR("ERROR: {}", v3_tri6_idx);

        // SPDLOG_INFO("sum: {}, {}, {}, vtx: {}", sum.x, sum.y, sum.z,
        //             cur_vtx_idx);
        // SPDLOG_INFO("sum: {}, {}, {}, vtx: {}", glm::normalize(sum).x,
        //             glm::normalize(sum).y, glm::normalize(sum).z,
        //             cur_vtx_idx);
        mesh.vertices[cur_vtx_idx].normal = glm::normalize(sum);

        // SPDLOG_INFO("cur vtx: {}", cur_vtx_idx);
        // SPDLOG_INFO("v3_tri5_idx: {}", v3_tri5_idx);
        // SPDLOG_INFO("tri1: ", v0_tri1_idx);
        // SPDLOG_INFO("tri2: ", v0_tri2_idx);
        // SPDLOG_INFO("tri3: ", v1_tri3_idx);
      }
    }
    // int i = 0;
    // for (auto &n : tri_normals) {
    //   SPDLOG_INFO("tri normal {} : {}, {}, {}", i, n.x, n.y, n.z);
    //   i++;
    // }
    return mesh;
  }
} // namespace Chemical
