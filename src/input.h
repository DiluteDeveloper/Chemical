#pragma once

#include <GLFW/glfw3.h>
#include <functional>

namespace Chemical {

    struct Input {
        static void Initialise(GLFWwindow* _window);

        static void SetCursorDisabled();
        static void SetCursorEnabled();
        // The data pointer acts as an identifier to be able to unsubscribe from this callback later.
        typedef std::function<void(int key, int scancode, int action, int mods, void* data)> KeyCallback;
        static void SubscribeToKeyCallback(const KeyCallback cb, void* data);
        static void UnsubscribeFromKeyCallback(void* data_ptr);

        // The data pointer acts as an identifier to be able to unsubscribe from this callback later.
        typedef std::function<void(double xpos, double ypos, void* data)> CursorPosCallback;
        static void SubscribeToCursorPosCallback(const CursorPosCallback cb, void* data);
        static void UnsubscribeFromCursorPosCallback(void* data_ptr);

        static bool GetKey(int key);

    };
}
