#include "chemical/core.h"

#include "chemical/window.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <spdlog/spdlog.h>
#include <stb_image/stb_image.h>

namespace Chemical {
  Core::Core() {
    spdlog::info("Initialising Chemical");

    spdlog::info("Initialising GLFW");
    if (!glfwInit()) {
      spdlog::critical("GLFW initialisation failed");
      throw std::runtime_error("GLFW initialisation failed");
    }
    spdlog::info("Setting GLFW window hints");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    std::optional<Window> opt_window = CreateNewWindow("Chemical 1.1.9", 1280, 720);

    if (!opt_window.has_value()) {
      spdlog::critical("Window initialisation failed");
      throw std::runtime_error("Window initialisation failed");
    }

    window = opt_window.value();

    renderer = std::make_unique<Graphics::SceneRenderer>();
    scene = std::make_unique<Scene>();

    stbi_set_flip_vertically_on_load(true);
  }
  Core::~Core() {

    spdlog::info("Terminating Chemical");
    delete renderer.release();
    delete scene.release();
    glfwTerminate();
  }

  void Core::SetGameLoopCallback(const std::function<void(Core &)> &callback) {

    game_loop_callback = callback;
  }
  void Core::StartGameLoop() {

    while (!WindowShouldClose(window)) {

      glClear(GL_COLOR_BUFFER_BIT);
      game_loop_callback(*this);

      renderer->RenderScene(*scene.get());

      glfwSwapBuffers(window);

      glfwPollEvents();
    }
  }

  void Core::SetBackgroundColour(const glm::vec3 &colour) {
    glClearColor(colour.r / 255.0f, colour.g / 255.0f, colour.b / 255.0f, 1.0f);
  }
  Scene &Core::GetScene() { return *scene.get(); }
} // namespace Chemical
