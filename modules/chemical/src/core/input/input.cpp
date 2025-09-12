#include "input.hpp"

namespace Chemical {

  namespace Input {

    int KeyEventSystem::key_states[348];

    std::vector<KeyEventSystem::KeyEventSubscriber>
        KeyEventSystem::key_event_subscribers;

    void KeyEventSystem::KeyCallback(GLFWwindow *window, int key, int scancode,
                                     int action, int mods) {

      for (auto &subscriber : key_event_subscribers) {
        subscriber(key, scancode, action, mods);
      }
    }
    void KeyEventSystem::SubscribeToKeyEvent(KeyEventSubscriber subscriber) {

      key_event_subscribers.emplace_back(subscriber);
    }
  } // namespace Input
} // namespace Chemical
