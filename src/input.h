#pragma once

#include <GLFW/glfw3.h>
#include <functional>
#include <unordered_map>

namespace Chemical {

    class Input {
    public:
        static void Initialise(GLFWwindow* window);

        // The data pointer acts as an identifier to be able to unsubscribe from this callback later.
        typedef std::function<void(int key, int scancode, int action, int mods, void* data)> KeyCallback;
        static void SubscribeToKeyCallback(const KeyCallback cb, void* data);
        static void UnsubscribeFromKeyCallback(void* data_ptr);

        // The data pointer acts as an identifier to be able to unsubscribe from this callback later.
        typedef std::function<void(double xpos, double ypos, void* data)> CursorPosCallback;
        static void SubscribeToCursorPosCallback(const CursorPosCallback cb, void* data);
        static void UnsubscribeFromCursorPosCallback(void* data_ptr);

        static bool GetKey(int key);
    private:
        static GLFWwindow* window;
        static std::unordered_map<void*, KeyCallback> key_callbacks;
        static std::unordered_map<void*, CursorPosCallback> cursor_pos_callbacks;


        static void OnGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void OnGLFWCursorPosCallback(GLFWwindow*, double xpos, double ypos);
    };
}
