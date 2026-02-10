#include "core/input/input.hpp"
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "gui/terrain_state_menu.hpp"
#include "rendering/shader.hpp"
#include "terrain_state.hpp"
#include "util/transform.hpp"
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "spdlog/spdlog.h"

#include "rendering/model_loader.hpp"
#include "util/camera.hpp"

#include "rendering/cube.hpp"
#include "window_handler.hpp"

#include "gui/setup.hpp"

using namespace Chemical;

bool menu_state = true;
std::unique_ptr<CameraController> camera;

GLFWwindow *window_ptr = nullptr;

std::string abs_res_dir =
    "/home/dilute/Documents/Dev/Chemical/modules/chemical/res";

void KeyCallback(int key, int scancode, int action, int mods) {

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    menu_state = !menu_state;
    if (menu_state) {
      glfwSetInputMode(window_ptr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      camera->enabled = false;
    } else {
      glfwSetInputMode(window_ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      camera->ResetMouse(window_ptr);
      camera->enabled = true;
    }
  }
}

int main() {

  spdlog::set_pattern("%^[%s] [%!] [%#] %$%v");

  int window_width = 1080;
  int window_height = 720;
  const char *window_title = "Game Development 0.17.5";

  WindowHandler window("Game Development", 1280, 720);
  window_ptr = window.window;

  window.SubscribeToKeyEvent(KeyCallback);

  GUI::SetupGUI(window.window);

  glViewport(0, 0, window_width, window_height);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.3, 0.3, 0.6, 1.0);

  // Input::KeyEventSystem::SubscribeToKeyEvent(KeyCallback);

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

  camera = std::make_unique<CameraController>(window.window, 0.3f);
  window.SubscribeToCursorPosEvent(
      [&](double x, double y) { camera->CursorPosCallback(x, y); });
  bool selection_first_collider = true;
  int collider1_idx = 0;
  int collider2_idx = 1;

  Util::TerrainState terrain;
  terrain.Generate();

  while (!glfwWindowShouldClose(window.window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GUI::BeginFrame();

    GUI::RenderTerrainStateMenu(terrain);

    camera->Update(window.window);
    shader.Bind();
    shader.SetUniformMatrix4FV(
        "v_view", 1, GL_FALSE,
        &glm::inverse(camera->transform.ToMatrix())[0][0]);
    shader.SetUniform3FV("viewPos", 1, &camera->transform.position[0]);

    glBindVertexArray(terrain.GetVAO());
    shader.SetUniformMatrix4FV("v_model", 1, GL_FALSE,
                               &Transform().ToMatrix()[0][0]);
    shader.SetUniform3F("material.diffuse", 1, 1, 1);
    shader.SetUniform1F("material.shininess", 16);

    glDrawElements(GL_TRIANGLES, terrain.GetIndiceCount(), GL_UNSIGNED_INT,
                   nullptr);
    glfwPollEvents();

    GUI::RenderFrame();

    glfwSwapBuffers(window.window);
  }
  GUI::CleanupGUI();

  return 0;
}
