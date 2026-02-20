#include "physics/simulation_controller.hpp"
#include "rendering/orbit_line_mesh.hpp"
#include <filesystem>
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "glm/ext/matrix_clip_space.hpp"

#include "rendering/icosphere.hpp"
#include "rendering/shader.hpp"
#include "util/transform.hpp"
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>

#include "spdlog/spdlog.h"

#include "util/camera.hpp"

#include "rendering/cube.hpp"
#include "window_handler.hpp"

#include "gui/rendering.hpp"
#include "gui/setup.hpp"

using namespace Chemical;

bool menu_state = true;
std::unique_ptr<CameraController> camera;

GLFWwindow *window_ptr = nullptr;

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

  SPDLOG_INFO("Application running in working directory \"{}\"",
              std::filesystem::current_path().c_str());

  int window_width = 1080;
  int window_height = 720;
  const char *window_title = "Game Development 0.17.5";

  WindowHandler window("Game Development", 1280, 720);
  window_ptr = window.window;

  window.SubscribeToKeyEvent(KeyCallback);

  GUI::SetupGUI(window.window);

  // GLfloat range[2];
  // glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, &range[0]);
  // SPDLOG_INFO("range : {} {}", range[0], range[1]);

  glViewport(0, 0, window_width, window_height);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.2, 0.2, 0.3, 1.0);

  ShaderTraits traits;
  std::ifstream file("res/shaders/LitShader.vs");

  if (!file) {
    SPDLOG_ERROR("Failed to read file \"no name\" : returning 0");
  }

  traits.vs_source = std::string((std::istreambuf_iterator<char>(file)),
                                 std::istreambuf_iterator<char>());

  file.close();

  std::ifstream file2("res/shaders/LitShader.fs");

  if (!file2) {
    SPDLOG_ERROR(R"(Failed to read file "" : returning 0)");
  }

  traits.fs_source = std::string((std::istreambuf_iterator<char>(file2)),
                                 std::istreambuf_iterator<char>());

  file2.close();
  Shader shader(traits);
  if (shader.compile_status == -1) {

    SPDLOG_ERROR("Shader failed to compile.");
    throw std::exception();
  }

  shader.Bind();

  ShaderTraits line_shader_traits;
  std::ifstream file3("res/shaders/LineShader.vs");

  if (!file3) {
    SPDLOG_ERROR("Failed to read file \"no name\" : returning 0");
  }

  line_shader_traits.vs_source =
      std::string((std::istreambuf_iterator<char>(file3)),
                  std::istreambuf_iterator<char>());

  file3.close();

  std::ifstream file4("res/shaders/LineShader.fs");

  if (!file4) {
    SPDLOG_ERROR(R"(Failed to read file "" : returning 0)");
  }

  line_shader_traits.fs_source =
      std::string((std::istreambuf_iterator<char>(file4)),
                  std::istreambuf_iterator<char>());

  file4.close();
  Shader line_shader(line_shader_traits);
  if (line_shader.compile_status == -1) {

    SPDLOG_ERROR("Shader failed to compile.");
    throw std::exception();
  }

  shader.Bind();

  glm::mat4 proj = glm::perspective(90.0f, 1280.0f / 720.0f, 0.1f, 100000.0f);
  shader.SetUniformMatrix4FV("v_proj", 1, GL_FALSE, &proj[0][0]);
  line_shader.Bind();
  line_shader.SetUniformMatrix4FV("v_proj", 1, GL_FALSE, &proj[0][0]);

  camera = std::make_unique<CameraController>(window.window, 10.06f);
  window.SubscribeToCursorPosEvent(
      [&](double x, double y) { camera->CursorPosCallback(x, y); });

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  Mesh icosphere = GenerateIcosphereSmoothNormals(15);

  Physics::SimulationController physics;

  unsigned int icosphere_vao = icosphere.AsVAO();

  GUI::RenderingSettings settings;
  settings.orbit_trail_length = 10000.0f;
  settings.orbit_trail_width = 1.0f;

  shader.SetUniform1UI("material.shininess", 16);

  while (!glfwWindowShouldClose(window.window)) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GUI::BeginFrame();

    GUI::RenderingSettings new_settings = GUI::RenderRenderingMenu(settings);
    if (new_settings.orbit_trail_length != settings.orbit_trail_length) {
      physics.orbit_line_a.SetSegmentCount(new_settings.orbit_trail_length);
      physics.orbit_line_b.SetSegmentCount(new_settings.orbit_trail_length);
    }
    settings = new_settings;
    glClearColor(settings.background_colour.r, settings.background_colour.g,
                 settings.background_colour.b, 1.0f);

    physics.Update();
    physics.orbit_line_a.Update(physics.positions_a);
    physics.orbit_line_b.Update(physics.positions_b);

    camera->Update(window.window);
    shader.Bind();
    shader.SetUniformMatrix4FV(
        "v_view", 1, GL_FALSE,
        &glm::inverse(camera->transform.ToMatrix())[0][0]);
    shader.SetUniform3FV("viewPos", 1, &camera->transform.position[0]);

    glBindVertexArray(icosphere_vao);

    shader.SetUniformMatrix4FV("v_model", 1, GL_FALSE,
                               &physics.GetActiveTransformA().ToMatrix()[0][0]);
    glLineWidth(1);

    shader.SetUniform3F("material.diffuse", settings.body_a_colour.r,
                        settings.body_a_colour.g, settings.body_a_colour.b);
    glDrawElements(GL_TRIANGLES, icosphere.indices.size(), GL_UNSIGNED_INT,
                   nullptr);
    shader.SetUniformMatrix4FV("v_model", 1, GL_FALSE,
                               &physics.GetActiveTransformB().ToMatrix()[0][0]);

    shader.SetUniform3F("material.diffuse", settings.body_b_colour.r,
                        settings.body_b_colour.g, settings.body_b_colour.b);
    glDrawElements(GL_TRIANGLES, icosphere.indices.size(), GL_UNSIGNED_INT,
                   nullptr);

    line_shader.Bind();
    line_shader.SetUniformMatrix4FV(
        "v_view", 1, GL_FALSE,
        &glm::inverse(camera->transform.ToMatrix())[0][0]);

    if (physics.IsRunning()) {
      glLineWidth(settings.orbit_trail_width);

      line_shader.SetUniform3F("f_colour", settings.trail_a_colour.r,
                               settings.trail_a_colour.g,
                               settings.trail_a_colour.b);
      physics.orbit_line_a.Draw(shader);
      line_shader.SetUniform3F("f_colour", settings.trail_b_colour.r,
                               settings.trail_b_colour.g,
                               settings.trail_b_colour.b);
      physics.orbit_line_b.Draw(shader);
    }

    glfwPollEvents();

    GUI::RenderFrame();

    glfwSwapBuffers(window.window);
  }
  GUI::CleanupGUI();

  return 0;
}
