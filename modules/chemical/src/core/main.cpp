#include "collision/box_collider.hpp"
#include "core/input/input.hpp"
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "gui/perlin.hpp"
#include "gui/transform.hpp"
#include "marching_cubes.hpp"
#include "rendering/material.hpp"
#include "rendering/shader.hpp"
#include "util/transform.hpp"
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "spdlog/spdlog.h"

#include "maths/perlin_noise.hpp"

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

std::string abs_res_dir =
    "/home/dilute/Documents/Dev/Chemical/modules/chemical/res";

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
  std::ifstream file(std::format("{}/shaders/LitShader.vs", abs_res_dir));

  if (!file) {
    SPDLOG_ERROR("Failed to read file \"no name\" : returning 0");
  }

  traits.vs_source = std::string((std::istreambuf_iterator<char>(file)),
                                 std::istreambuf_iterator<char>());

  file.close();

  std::ifstream file2(std::format("{}/shaders/LitShader.fs", abs_res_dir));

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
      std::format("{}/shaders/ColliderShader.vs", abs_res_dir));

  if (!collider_vs_file) {
    SPDLOG_ERROR(R"(Failed to read file "" : returning 0)");
  }

  traits.vs_source =
      std::string((std::istreambuf_iterator<char>(collider_vs_file)),
                  std::istreambuf_iterator<char>());

  collider_vs_file.close();

  std::ifstream collider_fs_file(
      std::format("{}/shaders/ColliderShader.fs", abs_res_dir));

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

  std::optional<Model> model_opt =
      ModelLoader::LoadModel("res/models/cube.dae");
  if (!model_opt)
    throw std::runtime_error("failed to load model");

  Model &model = model_opt.value();
  // unsigned int modelvao = model.meshes[0].AsVAO();

  glm::vec3 collision_colour = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 not_collision_colour = glm::vec3(1.0f, 0.0f, 0.0f);

  // for collider shader
  shader.SetUniform3F("material.diffuse", not_collision_colour.x,
                      not_collision_colour.y, not_collision_colour.z);
  shader.SetUniform1UI("material.shininess", 16);

  camera = std::make_unique<CameraController>(window, 0.3f);
  bool selection_first_collider = true;
  int collider1_idx = 0;
  int collider2_idx = 1;

  Mesh m = GenerateMarchingCubes();
  unsigned int m_vao = m.AsVAO();

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GUI::BeginFrame();

    GUI::RenderPerlinWindow();

    camera->Update(window);
    shader.Bind();
    shader.SetUniformMatrix4FV(
        "v_view", 1, GL_FALSE,
        &glm::inverse(camera->transform.ToMatrix())[0][0]);
    shader.SetUniform3FV("viewPos", 1, &camera->transform.position[0]);

    glBindVertexArray(m_vao);
    shader.SetUniformMatrix4FV("v_model", 1, GL_FALSE,
                               &Transform().ToMatrix()[0][0]);
    shader.SetUniform3F("material.diffuse", 1, 1, 1);
    shader.SetUniform1F("material.shininess", 16);

    glDrawElements(GL_TRIANGLES, m.indices.size(), GL_UNSIGNED_INT, nullptr);
    glfwPollEvents();

    GUI::RenderFrame();

    glfwSwapBuffers(window);
  }
  GUI::CleanupGUI();

  Window::DestroyGLFWWindow(window);
  return 0;
}
