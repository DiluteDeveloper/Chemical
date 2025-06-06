#include "chemical/core.h"

#include "chemical/io/json_scene.h"
#include "chemical/window.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/sinks/stdout_color_sinks-inl.h>
#include <spdlog/spdlog.h>
#include <stb_image/stb_image.h>

namespace Chemical {

  void Core::ConfigureSpdlog() const {
    spdlog::set_pattern("%^[%s] [%!] [%#] %$%v");
  }
  Core::Core(const char *window_title, glm::vec2 window_size) {
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
    renderer = std::make_unique<Graphics::Renderer>();

    SPDLOG_INFO("Initialising Scene");

    std::optional<Scene> scene =
        LoadSceneFromJSONFile("/mnt/storage/Chemical/Chemical/modules/chemical/res/json/scene.json");

    if (scene) {
      active_scene = std::make_unique<Scene>(std::move(scene.value()));
    }

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
      active_scene->UpdateScene();

      renderer->RenderScene(*active_scene.get());

      glfwSwapBuffers(window);

      glfwPollEvents();
    }
  }

  void Core::SetBackgroundColour(const glm::vec3 &colour) {
    SPDLOG_INFO("Setting OpenGL clear colour to [{}, {}, {}]", colour.r, colour.g, colour.b);
    glClearColor(colour.r / 255.0f, colour.g / 255.0f, colour.b / 255.0f, 1.0f);
  }

} // namespace Chemical
