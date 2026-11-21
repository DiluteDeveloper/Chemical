#pragma once

#include <GLFW/glfw3.h>
#include <functional>

namespace Chemical {

    class Input {
    public:
        Input(GLFWwindow* window);

        void SetCursorDisabled();
        void SetCursorEnabled();

        typedef std::function<void(int button, int action, int mods)> MouseButtonCallback;
        typedef std::function<void(double xoffset, double yoffset)> ScrollCallback;
        typedef std::function<void(double xpos, double ypos)> CursorPosCallback;
        typedef std::function<void(int key, int scancode, int action, int mods)> KeyCallback;

        void SubscribeToMouseButtonCallback(const MouseButtonCallback cb, void* identifier);
        void UnsubscribeFromMouseButtonCallback(void* identifier);

        void SubscribeToKeyCallback(const KeyCallback cb, void* identifier);
        void UnsubscribeFromKeyCallback(void* identifier);

        void SubscribeToCursorPosCallback(const CursorPosCallback cb, void* identifier);
        void UnsubscribeFromCursorPosCallback(void* identifier);

        void SubscribeToScrollCallback(const ScrollCallback cb, void* identifier);
        void UnsubscribeFromScrollCallback(void* identifier);

        bool GetKey(int key);

    private:
        std::unordered_map<void*, KeyCallback> key_callbacks;
        std::unordered_map<void*, CursorPosCallback> cursor_pos_callbacks;
        std::unordered_map<void*, MouseButtonCallback> mouse_button_callbacks;
        std::unordered_map<void*, ScrollCallback> scroll_callbacks;

        static void OnGLFWKeyCallback(GLFWwindow*, int key, int scancode, int action, int mods);
        static void OnGLFWCursorPosCallback(GLFWwindow*, double xpos, double ypos);
        static void OnGLFWMouseButtonCallback(GLFWwindow*, int button, int action, int mods);
        static void OnGLFWScrollCallback(GLFWwindow*, double xoffset, double yoffset);

        GLFWwindow* window;
    };
}
