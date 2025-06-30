#include "core/core.h"

#include "core/resource_manager.h"

#include <fstream>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <stdexcept>
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/sinks/stdout_color_sinks-inl.h>
#include <spdlog/spdlog.h>
#include <stb_image/stb_image.h>

namespace Chemical {

  using namespace std::placeholders;

  void Core::ConfigureSpdlog() const {
    spdlog::set_pattern("%^[%s] [%!] [%#] %$%v");
  }
  Core::Core(const std::string_view& window_title, const glm::vec2& window_size) {
    std::cout << "================================= " << window_title
              << " ====================================================" << std::endl;
    ConfigureSpdlog();

    SPDLOG_INFO("Initialising GLFW");
    if (!glfwInit()) {
      SPDLOG_CRITICAL("Failed to initialise Chemical : GLFW initialisation failed");
      throw std::runtime_error("GLFW initialisation failed");
    }
    SPDLOG_INFO("Configuring GLFW window hints");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    std::optional<Window> opt_window = CreateNewWindow(window_title, window_size.x, window_size.y);

    if (!opt_window.has_value()) {
      SPDLOG_CRITICAL("Failed to initialise Chemical : window creation failed");
      throw std::runtime_error("Window creation failed");
    }

    window = opt_window.value();

    SPDLOG_INFO("Initialising Input");
    input = std::make_unique<Input>(window);

    SPDLOG_INFO("Initialising Renderer");
    renderer = std::make_unique<OpenGL::Renderer>();

    ShaderTraits traits;
    std::ifstream file("/mnt/storage/Chemical/Chemical/modules/chemical/res/shaders/default_shader.vs");

    if (!file) {
      SPDLOG_ERROR(R"(Failed to read file "" : returning 0)");
    }

    traits.vs_source = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    file.close();

    std::ifstream file2("/mnt/storage/Chemical/Chemical/modules/chemical/res/shaders/default_shader.fs");

    if (!file2) {
      SPDLOG_ERROR(R"(Failed to read file "" : returning 0)");
    }

    traits.fs_source = std::string((std::istreambuf_iterator<char>(file2)), std::istreambuf_iterator<char>());

    file2.close();
    renderer->RegisterShader("default", traits);

    std::ifstream file3("/mnt/storage/Chemical/Chemical/modules/chemical/res/shaders/sin_shader.vs");

    if (!file3) {
      SPDLOG_ERROR(R"(Failed to read file "" : returning 0)");
    }

    traits.vs_source = std::string((std::istreambuf_iterator<char>(file3)), std::istreambuf_iterator<char>());

    file3.close();
    renderer->RegisterShader("sin_shader", traits);

    SPDLOG_INFO("Initialising Resource Manager");
    ResourceManager::SetResourceMasterPath("/mnt/storage/Chemical/Chemical/modules/chemical/res/");

    SPDLOG_INFO("Initialising Scene");
    active_scene = std::make_unique<Scene>();
    if (!ResourceManager::LoadAndRegisterScene("scenes/scene.json", *active_scene))
      throw std::runtime_error("Failed to load scene");
    if (!ResourceManager::LoadAndRegisterProject("project.json", *renderer))
      throw std::runtime_error("Failed to load project");

    SPDLOG_INFO("Configuring stbi_image");
    stbi_set_flip_vertically_on_load(true);
  }
  Core::~Core() {
    SPDLOG_INFO("Deleting Scene");
    delete active_scene.release();
    SPDLOG_INFO("Deleting Renderer");
    delete renderer.release();
    SPDLOG_INFO("Destroying window");
    DestroyWindow(window);
    SPDLOG_INFO("Terminating GLFW");
    glfwTerminate();
  }

  void Core::StartGameLoop() {

    while (!WindowShouldClose(window)) {

      glClear(GL_COLOR_BUFFER_BIT);
      auto renderables = active_scene->GetRenderables();
      renderer->RenderRenderables(*renderables);

      glfwSwapBuffers(window);

      glfwPollEvents();
    }
  }

  void Core::SetBackgroundColour(const glm::vec3& colour) {
    SPDLOG_INFO("Setting OpenGL clear colour to [{}, {}, {}]", colour.r, colour.g, colour.b);
    glClearColor(colour.r / 255.0f, colour.g / 255.0f, colour.b / 255.0f, 1.0f);
  }

} // namespace Chemical
