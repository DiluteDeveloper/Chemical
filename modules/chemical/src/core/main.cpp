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
      ModelLoader::LoadModel("res/models/test_scene.fbx");
  if (!model_opt)
    throw std::runtime_error("failed to load model");

  Model &model = model_opt.value();

  glm::mat4 modelmat = glm::mat4(1.0f);
  modelmat = glm::rotate(modelmat, glm::radians(-90.0f), glm::vec3(1, 0, 0));

  shader.SetUniform3F("material.diffuse", model.material.diffuse.x,
                      model.material.diffuse.y, model.material.diffuse.z);
  shader.SetUniform1UI("material.shininess", model.material.shininess);

  // VAO then index count
  std::vector<std::pair<unsigned int, unsigned int>> gl_mesh_data;

  for (const Mesh &mesh : model.meshes) {

    gl_mesh_data.emplace_back(mesh.AsVAO(), mesh.indices.size());
  }

  glm::vec3 collision_colour = glm::vec3(0.0f, 1.0f, 0.0f);
  BoxCollider3D collider_1(glm::vec3(2, 3, 5), glm::vec3(0, 15, 0));
  BoxCollider3D collider_2(glm::vec3(5, 10, 12), glm::vec3(7, 15, 3));
  Mesh mesh_1 = collider_1.AsMesh();
  Mesh mesh_2 = collider_2.AsMesh();
  unsigned int collider_vao_1 = mesh_1.AsVAO();
  unsigned int collider_vao_2 = mesh_2.AsVAO();

  bool cursor_disabled = true;

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.Update(window);
    shader.Bind();
    shader.SetUniformMatrix4FV(
        "v_view", 1, GL_FALSE,
        &glm::inverse(camera.transform.ToMatrix())[0][0]);
    shader.SetUniform3FV("viewPos", 1, &camera.transform.position[0]);
    shader.SetUniformMatrix4FV("v_model", 1, GL_FALSE, &modelmat[0][0]);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
      if (cursor_disabled)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      else
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      cursor_disabled = !cursor_disabled;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT)) {
      collider_1.position.x -= 0.05f;
    } else if (glfwGetKey(window, GLFW_KEY_RIGHT))
      collider_1.position.x += 0.05f;
    if (glfwGetKey(window, GLFW_KEY_UP)) {
      collider_1.position.z -= 0.05f;
    } else if (glfwGetKey(window, GLFW_KEY_DOWN))
      collider_1.position.z += 0.05f;

    for (const auto &mesh_data : gl_mesh_data) {
      glBindVertexArray(mesh_data.first);
      glDrawElements(GL_TRIANGLES, mesh_data.second, GL_UNSIGNED_INT, nullptr);
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    collider_shader.Bind();
    collider_shader.SetUniformMatrix4FV(
        "v_view", 1, GL_FALSE,
        &glm::inverse(camera.transform.ToMatrix())[0][0]);
    collider_shader.SetUniform3FV("v_collider_position", 1,
                                  &collider_1.position[0]);
    collider_shader.SetUniform3FV("f_colour", 1, &collision_colour[0]);
    glBindVertexArray(collider_vao_1);
    glDrawElements(GL_TRIANGLES, mesh_1.indices.size(), GL_UNSIGNED_INT,
                   nullptr);
    collider_shader.SetUniform3FV("v_collider_position", 1,
                                  &collider_2.position[0]);
    glBindVertexArray(collider_vao_2);
    glDrawElements(GL_TRIANGLES, mesh_2.indices.size(), GL_UNSIGNED_INT,
                   nullptr);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (collider_1.IsCollidingWith(collider_2)) {
      collision_colour = glm::vec3(1.0f, 0.0f, 0.0f);
    } else
      collision_colour = glm::vec3(0.0f, 1.0f, 0.0f);

    glfwPollEvents();

    glfwSwapBuffers(window);
  }

  Window::DestroyGLFWWindow(window);
  return 0;
}
