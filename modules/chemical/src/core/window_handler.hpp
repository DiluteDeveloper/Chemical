
#include "glfw/glfw3.h"
#include <functional>
#include <string_view>

namespace Chemical {

  // Only one instance of this
  class WindowHandler {
  public:
    GLFWwindow *window;

    WindowHandler(const std::string_view &title, unsigned int width,
                  unsigned int height);
    ~WindowHandler();

    WindowHandler(const WindowHandler &) = delete;
    WindowHandler &operator=(const WindowHandler &) = delete;

    WindowHandler(WindowHandler &&) = delete;
    WindowHandler &operator=(WindowHandler &&) = delete;

    using KeyEventSubscriber = std::function<void(int, int, int, int)>;
    using CursorPosEventSubscriber = std::function<void(double, double)>;

    void SubscribeToKeyEvent(KeyEventSubscriber subscriber);
    void SubscribeToCursorPosEvent(CursorPosEventSubscriber subscriber);

  private:
    static void FwdGLFWKeyCallback(GLFWwindow *window, int key, int scancode,
                                   int action, int mods);
    void GLFWKeyCallback(int key, int scancode, int action, int mods);
    std::vector<KeyEventSubscriber> key_event_subscribers;

    static void FwdGLFWCursorPosCallback(GLFWwindow *window, double x,
                                         double y);
    void GLFWCursorPosCallback(double x, double y);
    std::vector<CursorPosEventSubscriber> cursor_pos_event_subscribers;
  };
} // namespace Chemical
