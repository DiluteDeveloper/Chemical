
#include "glfw/glfw3.h"
#include <string_view>

namespace Chemical {

  namespace Window {

    extern GLFWwindow *
    InitialiseGLContextAndGLFWWindow(const std::string_view &title,
                                     unsigned int width, unsigned int height);
    extern void DestroyGLFWWindow(GLFWwindow *window);
  } // namespace Window
} // namespace Chemical
