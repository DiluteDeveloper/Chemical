#include "core/window.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <spdlog/spdlog.h>

namespace Chemical {

  void GLFWErrorCallback(int error_code, const char* description) {
    SPDLOG_ERROR("GLFW error {} occurred : {}", error_code, description);
  }
  std::optional<Window> CreateNewWindow(const std::string_view& title, unsigned int width,
                                        unsigned int height) {
    SPDLOG_INFO(R"(Creating window "{}")", title);

    glfwSetErrorCallback(GLFWErrorCallback);

    GLFWwindow* window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

    if (!window) {
      SPDLOG_CRITICAL(R"(Failed to create window "{}" : glfwCreateWindow failed)", title);
      return nullptr;
    }
    glfwMakeContextCurrent(window);

    SPDLOG_INFO(R"(Loading OpenGL functions for window "{}")", title);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      SPDLOG_CRITICAL(R"(Failed to create window "{}" : gladLoadGLLoader failed)", title);
      return nullptr;
    }

    SPDLOG_INFO(R"(Turning on VSync for window "{}")", title);
    glfwSwapInterval(1);

    return (Window)window;
  }

  void DestroyWindow(Window window) {
    glfwDestroyWindow(window);
  }

  void BindWindow(Window window) {
    glfwMakeContextCurrent(window);
  }

  bool WindowShouldClose(Window window) {
    return glfwWindowShouldClose(window);
  }
} // namespace Chemical
