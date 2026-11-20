#pragma once

#include <GLFW/glfw3.h>
#include <functional>

namespace Chemical {

    namespace Input {
        extern void Initialise(GLFWwindow* _window);

        extern void SetCursorDisabled();
        extern void SetCursorEnabled();
        // The data pointer acts as an identifier to be able to unsubscribe from this callback later.
        typedef std::function<void(int key, int scancode, int action, int mods, void* data)> KeyCallback;
        extern void SubscribeToKeyCallback(const KeyCallback cb, void* data);
        extern void UnsubscribeFromKeyCallback(void* data_ptr);

        // The data pointer acts as an identifier to be able to unsubscribe from this callback later.
        typedef std::function<void(double xpos, double ypos, void* data)> CursorPosCallback;
        extern void SubscribeToCursorPosCallback(const CursorPosCallback cb, void* data);
        extern void UnsubscribeFromCursorPosCallback(void* data_ptr);

        typedef std::function<void(int button, int action, int mods, void* data)> MouseButtonCallback;
        extern void SubscribeToMouseButtonCallback(const MouseButtonCallback cb, void* data);
        extern void UnsubscribeFromMouseButtonCallback(void* data_ptr);

        typedef std::function<void(double xoffset, double yoffset, void* data)> ScrollCallback;
        extern void SubscribeToScrollCallback(const ScrollCallback cb, void* data);
        extern void UnsubscribeFromScrollCallback(void* data_ptr);

        extern bool GetKey(int key);

    };
}
