#pragma once

#include <GLFW/glfw3.h>
#include <functional>

namespace Chemical {

    namespace Input {
        extern void Initialise(GLFWwindow* _window);

        extern void SetCursorDisabled();
        extern void SetCursorEnabled();
        // The data pointer acts as an identifier to be able to unsubscribe from this callback later.
        typedef std::function<void(int key, int scancode, int action, int mods)> KeyCallback;
        extern void SubscribeToKeyCallback(const KeyCallback cb, void* identifier);
        extern void UnsubscribeFromKeyCallback(void* identifier);

        // The data pointer acts as an identifier to be able to unsubscribe from this callback later.
        typedef std::function<void(double xpos, double ypos)> CursorPosCallback;
        extern void SubscribeToCursorPosCallback(const CursorPosCallback cb, void* identifier);
        extern void UnsubscribeFromCursorPosCallback(void* identifier);

        typedef std::function<void(int button, int action, int mods)> MouseButtonCallback;
        extern void SubscribeToMouseButtonCallback(const MouseButtonCallback cb, void* identifier);
        extern void UnsubscribeFromMouseButtonCallback(void* identifier);

        typedef std::function<void(double xoffset, double yoffset)> ScrollCallback;
        extern void SubscribeToScrollCallback(const ScrollCallback cb, void* identifier);
        extern void UnsubscribeFromScrollCallback(void* identifier);

        extern bool GetKey(int key);

    };
}
