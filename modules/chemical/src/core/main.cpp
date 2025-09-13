#include "collision/box_collider.hpp"
#include "core/input/input.hpp"
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "gui/transform.hpp"
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

#include "gui/setup.hpp"

#include "collision/mesh_collider.h"

using namespace Chemical;

bool menu_state = true;
GLFWwindow *window = nullptr;
std::unique_ptr<CameraController> camera;

void KeyCallback(int key, int scancode, int action, int mods) {

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    menu_state = !menu_state;
    if (menu_state)
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    else {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      camera->ResetMouse(window);
    }
  }
}
int main() {
  spdlog::set_pattern("%^[%s] [%!] [%#] %$%v");

  int window_width = 1080;
  int window_height = 720;
  const char *window_title = "Game Development 0.17.5";

  window = Window::InitialiseGLContextAndGLFWWindow(window_title, window_width,
                                                    window_height);

  GUI::SetupGUI(window);

  glViewport(0, 0, window_width, window_height);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.3, 0.3, 0.6, 1.0);

  glfwSetKeyCallback(window, Input::KeyEventSystem::KeyCallback);

  Input::KeyEventSystem::SubscribeToKeyEvent(KeyCallback);

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

  // Material material;
  // material.ambient = glm::vec3(0.2, 0.2, 0.2);
  // material.diffuse = glm::vec3(1.0, 0.3, 0.9);
  // material.specular = glm::vec3(1.0, 1.0, 1.0);
  // material.shininess = 32;

  std::optional<Model> model_opt =
      ModelLoader::LoadModel("res/models/roads.fbx");
  if (!model_opt)
    throw std::runtime_error("failed to load model");

  Model &model = model_opt.value();

  glm::vec3 collision_colour = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 not_collision_colour = glm::vec3(1.0f, 0.0f, 0.0f);

  // for collider shader
  shader.SetUniform3F("material.diffuse", not_collision_colour.x,
                      not_collision_colour.y, not_collision_colour.z);
  shader.SetUniform1UI("material.shininess", 16);

  // VAO then index count
  std::vector<std::pair<unsigned int, unsigned int>> gl_mesh_data;

  std::vector<std::pair<Transform *, std::string *>> transforms;

  for (Mesh &mesh : model.meshes) {
    mesh.transform.rotation.x += 270.0f;
    transforms.emplace_back(std::make_pair(&mesh.transform, &mesh.name));

    gl_mesh_data.emplace_back(mesh.AsVAO(), mesh.indices.size());
  }

  camera = std::make_unique<CameraController>(window, 0.03f);

  // BoxCollider3D c1(0.5f);
  // Mesh c1_mesh = c1.AsMesh();
  // c1_mesh.name = "c1";
  // transforms.emplace_back(std::make_pair(&c1_mesh.transform, &c1_mesh.name));
  // BoxCollider3D c2(0.5f);
  // Mesh c2_mesh = c2.AsMesh();
  // c2_mesh.name = "c2";
  // transforms.emplace_back(std::make_pair(&c2_mesh.transform, &c2_mesh.name));
  //
  bool selection_first_collider = true;
  int collider1_idx = 0;
  int collider2_idx = 1;

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GUI::BeginFrame();

    int idx = GUI::RenderTransformWindow(transforms);
    if (idx != -1) {
      SPDLOG_INFO("{}", idx);
      if (selection_first_collider)
        collider1_idx = idx;
      else
        collider2_idx = idx;
      selection_first_collider = !selection_first_collider;
    }

    if (!menu_state)
      camera->Update(window);
    shader.Bind();
    shader.SetUniformMatrix4FV(
        "v_view", 1, GL_FALSE,
        &glm::inverse(camera->transform.ToMatrix())[0][0]);
    shader.SetUniform3FV("viewPos", 1, &camera->transform.position[0]);

    // if (glfwGetKey(window, GLFW_KEY_LEFT)) {
    //   model.meshes[0].transform.pos=
    //       glm::translate(model.meshes[0].model, glm::vec3(-0.05f, 0.0f,
    //       0.0f));
    // } else if (glfwGetKey(window, GLFW_KEY_RIGHT))
    //   model.meshes[0].model =
    //       glm::translate(model.meshes[0].model, glm::vec3(0.05f, 0.0f,
    //       0.0f));
    // if (glfwGetKey(window, GLFW_KEY_UP)) {
    //   model.meshes[0].model =
    //       glm::translate(model.meshes[0].model, glm::vec3(0.0f, 0.0f,
    //       -0.05f));
    // } else if (glfwGetKey(window, GLFW_KEY_DOWN))
    //   model.meshes[0].model =
    //       glm::translate(model.meshes[0].model, glm::vec3(0.0f, 0.0f,
    //       0.05f));

    for (size_t i = 0; i < model.meshes.size(); i++) {
      shader.SetUniformMatrix4FV("v_model", 1, GL_FALSE,
                                 &model.meshes[i].transform.ToMatrix()[0][0]);
      shader.SetUniform3F("material.diffuse",
                          model.meshes[i].material.diffuse.r,
                          model.meshes[i].material.diffuse.g,
                          model.meshes[i].material.diffuse.b);
      shader.SetUniform1F("material.shininess",
                          model.meshes[i].material.shininess);

      if (i == collider1_idx || i == collider2_idx)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glBindVertexArray(gl_mesh_data[i].first);
      glDrawElements(GL_TRIANGLES, gl_mesh_data[i].second, GL_UNSIGNED_INT,
                     nullptr);
    }

    if (glfwGetKey(window, GLFW_KEY_X)) {
      if (IsColliding_SAT(model.meshes[collider1_idx].vertices,
                          model.meshes[collider1_idx].transform.ToMatrix(),
                          model.meshes[collider2_idx].vertices,
                          model.meshes[collider2_idx].transform.ToMatrix())) {
        SPDLOG_INFO("Colliding!");
      } else
        SPDLOG_INFO("Not colliding!");
    }

    // temporary solution
    // c1.position = c1_mesh.transform.position;
    // c2.position = c2_mesh.transform.position;
    //
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // collider_shader.Bind();
    // collider_shader.SetUniformMatrix4FV(
    //     "v_view", 1, GL_FALSE,
    //     &glm::inverse(camera->transform.ToMatrix())[0][0]);
    // collider_shader.SetUniform3FV("v_collider_position", 1, &c1.position[0]);
    // collider_shader.SetUniform3FV("f_colour", 1, &collision_colour[0]);
    // glBindVertexArray(c1_mesh.AsVAO());
    // glDrawElements(GL_TRIANGLES, c1_mesh.indices.size(), GL_UNSIGNED_INT,
    //                nullptr);
    // collider_shader.SetUniform3FV("v_collider_position", 1, &c2.position[0]);
    // glBindVertexArray(c2_mesh.AsVAO());
    // glDrawElements(GL_TRIANGLES, c2_mesh.indices.size(), GL_UNSIGNED_INT,
    //                nullptr);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // if (c1.IsCollidingWith(c2)) {
    //   collision_colour = glm::vec3(0.0f, 1.0f, 0.0f);
    // } else
    //   collision_colour = glm::vec3(1.0f, 0.0f, 0.0f);

    glfwPollEvents();

    GUI::RenderFrame();

    glfwSwapBuffers(window);
  }

  GUI::CleanupGUI();

  Window::DestroyGLFWWindow(window);
  return 0;
}
