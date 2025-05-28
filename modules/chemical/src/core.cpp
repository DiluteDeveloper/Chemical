#include "chemical/core.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <spdlog/spdlog.h>

namespace Chemical {

  void InitialiseChemical() {
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
  }

  void TerminateChemical() {
    spdlog::info("Terminating Chemical");
    glfwTerminate();
  }

  void PollEvents() { glfwPollEvents(); }
} // namespace Chemical
