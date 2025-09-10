#include "collision/box_collider.hpp"
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "rendering/material.hpp"
#include "rendering/shader.hpp"
#include "util/transform.hpp"
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "spdlog/spdlog.h"

#include "rendering/model_loader.hpp"
#include "util/camera.hpp"

#include "rendering/cube.hpp"
#include "window.hpp"

#include "collision/mesh_collider.h"

using namespace Chemical;

int main() {
  spdlog::set_pattern("%^[%s] [%!] [%#] %$%v");

  GLFWwindow *window = Window::InitialiseGLContextAndGLFWWindow(
      "Game Development 0.17.0", 1080, 720);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.3, 0.3, 0.6, 1.0);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  ShaderTraits traits;
  std::ifstream file("/mnt/storage/Chemical/Chemical/modules/chemical/res/"
                     "shaders/LitShader.vs");

  if (!file) {
    SPDLOG_ERROR(R"(Failed to read file "" : returning 0)");
  }

  traits.vs_source = std::string((std::istreambuf_iterator<char>(file)),
                                 std::istreambuf_iterator<char>());

  file.close();

  std::ifstream file2("/mnt/storage/Chemical/Chemical/modules/chemical/res/"
                      "shaders/LitShader.fs");

  if (!file2) {
    SPDLOG_ERROR(R"(Failed to read file "" : returning 0)");
  }

  traits.fs_source = std::string((std::istreambuf_iterator<char>(file2)),
                                 std::istreambuf_iterator<char>());

  file2.close();
  Shader shader(traits);
  if (shader.compile_status == -1)
    throw std::runtime_error("Shader failed to compile");

  shader.Bind();

  glm::mat4 proj = glm::perspective(90.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);
  shader.SetUniformMatrix4FV("v_proj", 1, GL_FALSE, &proj[0][0]);

  std::ifstream collider_vs_file(
      "/mnt/storage/Chemical/Chemical/modules/chemical/res/"
      "shaders/ColliderShader.vs");

  if (!collider_vs_file) {
    SPDLOG_ERROR(R"(Failed to read file "" : returning 0)");
  }

  traits.vs_source =
      std::string((std::istreambuf_iterator<char>(collider_vs_file)),
                  std::istreambuf_iterator<char>());

  collider_vs_file.close();

  std::ifstream collider_fs_file(
      "/mnt/storage/Chemical/Chemical/modules/chemical/res/"
      "shaders/ColliderShader.fs");

  if (!collider_fs_file) {
    SPDLOG_ERROR(R"(Failed to read file "" : returning 0)");
  }

  traits.fs_source =
      std::string((std::istreambuf_iterator<char>(collider_fs_file)),
                  std::istreambuf_iterator<char>());

  collider_fs_file.close();
  Shader collider_shader(traits);
  if (collider_shader.compile_status == -1)
    throw std::runtime_error("collider Shader failed to compile");

  collider_shader.Bind();

  collider_shader.SetUniformMatrix4FV("v_proj", 1, GL_FALSE, &proj[0][0]);

  CameraController camera(window);

  // Material material;
  // material.ambient = glm::vec3(0.2, 0.2, 0.2);
  // material.diffuse = glm::vec3(1.0, 0.3, 0.9);
  // material.specular = glm::vec3(1.0, 1.0, 1.0);
  // material.shininess = 32;

  std::optional<Model> model_opt =
      ModelLoader::LoadModel("res/models/test2.fbx");
  if (!model_opt)
    throw std::runtime_error("failed to load model");

  Model &model = model_opt.value();

  glm::vec3 collision_colour = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 not_collision_colour = glm::vec3(1.0f, 0.0f, 0.0f);

  shader.SetUniform3F("material.diffuse", not_collision_colour.x,
                      not_collision_colour.y, not_collision_colour.z);
  shader.SetUniform1UI("material.shininess", model.material.shininess);

  // VAO then index count
  std::vector<std::pair<unsigned int, unsigned int>> gl_mesh_data;

  for (Mesh &mesh : model.meshes) {
    mesh.model = glm::rotate(mesh.model, glm::radians(90.0f),
                             glm::vec3(1.0f, 0.0f, 0.0f));

    // SPDLOG_INFO("{}, {}, {}", mesh.vertices[0].position.x,
    //             mesh.vertices[0].position.y, mesh.vertices[0].position.z);
    // SPDLOG_INFO("{}, {}, {}", mesh.vertices[1].position.x,
    //             mesh.vertices[1].position.y, mesh.vertices[1].position.z);
    // SPDLOG_INFO("{}, {}, {}", mesh.vertices[2].position.x,
    //             mesh.vertices[2].position.y, mesh.vertices[2].position.z);
    gl_mesh_data.emplace_back(mesh.AsVAO(), mesh.indices.size());
  }

  bool cursor_disabled = true;

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.Update(window);
    shader.Bind();
    shader.SetUniformMatrix4FV(
        "v_view", 1, GL_FALSE,
        &glm::inverse(camera.transform.ToMatrix())[0][0]);
    shader.SetUniform3FV("viewPos", 1, &camera.transform.position[0]);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
      if (cursor_disabled)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      else
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      cursor_disabled = !cursor_disabled;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT)) {
      model.meshes[0].model =
          glm::translate(model.meshes[0].model, glm::vec3(-0.05f, 0.0f, 0.0f));
    } else if (glfwGetKey(window, GLFW_KEY_RIGHT))
      model.meshes[0].model =
          glm::translate(model.meshes[0].model, glm::vec3(0.05f, 0.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_UP)) {
      model.meshes[0].model =
          glm::translate(model.meshes[0].model, glm::vec3(0.0f, 0.0f, -0.05f));
    } else if (glfwGetKey(window, GLFW_KEY_DOWN))
      model.meshes[0].model =
          glm::translate(model.meshes[0].model, glm::vec3(0.0f, 0.0f, 0.05f));

    for (size_t i = 0; i < model.meshes.size(); i++) {
      shader.SetUniformMatrix4FV("v_model", 1, GL_FALSE,
                                 &model.meshes[i].model[0][0]);
      glBindVertexArray(gl_mesh_data[i].first);
      glDrawElements(GL_TRIANGLES, gl_mesh_data[i].second, GL_UNSIGNED_INT,
                     nullptr);
    }

    if (glfwGetKey(window, GLFW_KEY_X)) {
      if (IsColliding_SAT(model.meshes[0].vertices, model.meshes[0].model,
                          model.meshes[1].vertices, model.meshes[1].model)) {
        shader.SetUniform3F("material.diffuse", collision_colour.x,
                            collision_colour.y, collision_colour.z);
      } else {
        shader.SetUniform3F("material.diffuse", not_collision_colour.x,
                            not_collision_colour.y, not_collision_colour.z);
      }
    }

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // collider_shader.Bind();
    // collider_shader.SetUniformMatrix4FV(
    //     "v_view", 1, GL_FALSE,
    //     &glm::inverse(camera.transform.ToMatrix())[0][0]);
    // collider_shader.SetUniform3FV("v_collider_position", 1,
    //                               &collider_1.position[0]);
    // collider_shader.SetUniform3FV("f_colour", 1, &collision_colour[0]);
    // glBindVertexArray(collider_vao_1);
    // glDrawElements(GL_TRIANGLES, mesh_1.indices.size(), GL_UNSIGNED_INT,
    //                nullptr);
    // collider_shader.SetUniform3FV("v_collider_position", 1,
    //                               &collider_2.position[0]);
    // glBindVertexArray(collider_vao_2);
    // glDrawElements(GL_TRIANGLES, mesh_2.indices.size(), GL_UNSIGNED_INT,
    //                nullptr);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //
    // if (collider_1.IsCollidingWith(collider_2)) {
    //   collision_colour = glm::vec3(1.0f, 0.0f, 0.0f);
    // } else
    //   collision_colour = glm::vec3(0.0f, 1.0f, 0.0f);

    glfwPollEvents();

    glfwSwapBuffers(window);
  }

  Window::DestroyGLFWWindow(window);
  return 0;
}
