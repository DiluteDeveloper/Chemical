#include "window_handler.hpp"

#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "spdlog/spdlog.h"

namespace Chemical {

  void GLFWErrorCallback(int error_code, const char *description) {
    SPDLOG_ERROR("GLFW error {} occurred : {}", error_code, description);
  }

  void WindowHandler::FwdGLFWKeyCallback(GLFWwindow *window, int key,
                                         int scancode, int action, int mods) {

    WindowHandler *wh =
        static_cast<WindowHandler *>(glfwGetWindowUserPointer(window));
    wh->GLFWKeyCallback(key, scancode, action, mods);
  }

  void WindowHandler::GLFWKeyCallback(int key, int scancode, int action,
                                      int mods) {
    for (auto key_event_subscriber : key_event_subscribers) {
      key_event_subscriber(key, scancode, action, mods);
    }
  }

  void WindowHandler::FwdGLFWCursorPosCallback(GLFWwindow *window, double x,
                                               double y) {
    WindowHandler *wh =
        static_cast<WindowHandler *>(glfwGetWindowUserPointer(window));
    wh->GLFWCursorPosCallback(x, y);
  }
  void WindowHandler::GLFWCursorPosCallback(double x, double y) {

    for (auto cursor_pos_event_subscriber : cursor_pos_event_subscribers) {
      cursor_pos_event_subscriber(x, y);
    }
  }

  void WindowHandler::SubscribeToKeyEvent(KeyEventSubscriber subscriber) {
    key_event_subscribers.emplace_back(subscriber);
  }
  void WindowHandler::SubscribeToCursorPosEvent(
      CursorPosEventSubscriber subscriber) {
    cursor_pos_event_subscribers.emplace_back(subscriber);
  }

  WindowHandler::WindowHandler(const std::string_view &title,
                               unsigned int width, unsigned int height) {
    SPDLOG_INFO(R"(Creating window "{}")", title);

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

    if (!window) {
      SPDLOG_CRITICAL(
          R"(Failed to create window "{}" : glfwCreateWindow failed)", title);
      throw std::exception();
    }
    glfwMakeContextCurrent(window);

    SPDLOG_INFO(R"(Loading OpenGL functions for window "{}")", title);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      SPDLOG_CRITICAL(
          R"(Failed to create window "{}" : gladLoadGLLoader failed)", title);
      throw std::exception();
    }

    SPDLOG_INFO(R"(Turning on VSync for window "{}")", title);
    glfwSwapInterval(0);

    glfwSetWindowUserPointer(window, this);

    glfwSetErrorCallback(GLFWErrorCallback);
    glfwSetKeyCallback(window, FwdGLFWKeyCallback);
    glfwSetCursorPosCallback(window, FwdGLFWCursorPosCallback);
  }

  WindowHandler::~WindowHandler() {
    glfwDestroyWindow(window);
  }

} // namespace Chemical
