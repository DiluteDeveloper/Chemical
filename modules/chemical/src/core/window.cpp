#include "window.hpp"

#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "spdlog/spdlog.h"

namespace Chemical {

  namespace Window {

    void GLFWErrorCallback(int error_code, const char *description) {
      SPDLOG_ERROR("GLFW error {} occurred : {}", error_code, description);
    }
    GLFWwindow *InitialiseGLContextAndGLFWWindow(const std::string_view &title,
                                                 unsigned int width,
                                                 unsigned int height) {
      SPDLOG_INFO(R"(Creating window "{}")", title);

      glfwInit();

      glfwSetErrorCallback(GLFWErrorCallback);

      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

      int count;
      GLFWmonitor **monitors = glfwGetMonitors(&count);

      int xpos, ypos, mwidth, mheight;
      glfwGetMonitorWorkarea(monitors[0], &xpos, &ypos, &mwidth, &mheight);
      SPDLOG_INFO("M0: {}, {}, {}, {}", xpos, ypos, mwidth, mheight);
      glfwGetMonitorWorkarea(monitors[1], &xpos, &ypos, &mwidth, &mheight);

      SPDLOG_INFO("M0: {}, {}, {}, {}", xpos, ypos, mwidth, mheight);

      float xscale, yscale;

      glfwGetMonitorContentScale(monitors[0], &xscale, &yscale);
      SPDLOG_INFO("M0 contentscale: {}, {} ", xscale, yscale);
      glfwGetMonitorContentScale(monitors[1], &xscale, &yscale);
      SPDLOG_INFO("M0 contentscale: {}, {} ", xscale, yscale);
      GLFWwindow *window =
          glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

      if (!window) {
        SPDLOG_CRITICAL(
            R"(Failed to create window "{}" : glfwCreateWindow failed)", title);
        return nullptr;
      }
      glfwMakeContextCurrent(window);

      SPDLOG_INFO(R"(Loading OpenGL functions for window "{}")", title);
      if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        SPDLOG_CRITICAL(
            R"(Failed to create window "{}" : gladLoadGLLoader failed)", title);
        return nullptr;
      }

      SPDLOG_INFO(R"(Turning on VSync for window "{}")", title);
      glfwSwapInterval(1);

      return window;
    }

    void DestroyGLFWWindow(GLFWwindow *window) {
      glfwDestroyWindow(window);
    }
  } // namespace Window

} // namespace Chemical
