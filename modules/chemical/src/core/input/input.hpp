#pragma once

#include <functional>
#include <vector>
struct GLFWwindow;

namespace Chemical {

  namespace Input {
    // Coupled with GLFW enums and events
    class KeyEventSystem {

      using KeyEventSubscriber = std::function<void(int, int, int, int)>;

      static int key_states[];

      static std::vector<KeyEventSubscriber> key_event_subscribers;

    public:
      static void KeyCallback(GLFWwindow *window, int key, int scancode,
                              int action, int mods);

      static void SubscribeToKeyEvent(KeyEventSubscriber subscriber);
    };
  } // namespace Input

} // namespace Chemical
