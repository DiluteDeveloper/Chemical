#include "chemical/input.h"

#include <glfw/glfw3.h>

namespace Chemical {
  Input::Input(Window window) : window(window) {}

  Input::KeyState Input::GetKey(Key key) {

    // could be unsafe?
    return static_cast<Input::KeyState>(glfwGetKey(window, (int)key));
  }
  // Input::KeyState Input::GetKeyDown(Key key) {
  //
  //   return static_cast<Input::KeyState>(glfwGetKeyDown(window, (int)key));
  // }
  //
  // Input::KeyState Input::GetKeyUp(Key key) {}
} // namespace Chemical
