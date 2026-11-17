#include <unordered_map>

#include "input.h"
#include "GLFW/glfw3.h"

namespace Chemical {

        std::unordered_map<void*, Input::KeyCallback> key_callbacks;
        std::unordered_map<void*, Input::CursorPosCallback> cursor_pos_callbacks;
        GLFWwindow* window;


        bool Input::GetKey(int key) {
            return glfwGetKey(window, key);
        }

        void Input::SubscribeToKeyCallback(const KeyCallback cb, void* data) {
            key_callbacks.emplace(data, cb);
        }
        void Input::UnsubscribeFromKeyCallback(void* data_ptr) {
            key_callbacks.erase(data_ptr); 
        }

        void Input::SubscribeToCursorPosCallback(const CursorPosCallback cb, void* data) {
            cursor_pos_callbacks.emplace(data, cb);
        }
        void Input::UnsubscribeFromCursorPosCallback(void* data_ptr) {
            cursor_pos_callbacks.erase(data_ptr); 
        }

        void OnGLFWMouseButtonCallback(GLFWwindow*, int, int, int) {

        }
#ifdef __EMSCRIPTEN__
        void EmscriptenActivateMouseOnGLFWMouseButtonCallback(GLFWwindow*, int, int, int) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetMouseButtonCallback(window, OnGLFWMouseButtonCallback);
        }
#endif

        void Input::SetCursorDisabled() {
    #ifdef WSL
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
#else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    #endif
        } 
        void Input::SetCursorEnabled() {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        void OnGLFWKeyCallback(GLFWwindow*, int key, int scancode, int action, int mods) {
            for(auto [data, cb] : key_callbacks) {
                cb(key, scancode, action, mods, data);
            }
        }
        void OnGLFWCursorPosCallback(GLFWwindow*, double xpos, double ypos) {

            for(auto [data, cb] : cursor_pos_callbacks) {
                cb(xpos, ypos, data);
            }
        }
        void Input::Initialise(GLFWwindow* _window) {
            window = _window;

            glfwSetKeyCallback(window, OnGLFWKeyCallback);
            glfwSetCursorPosCallback(window, OnGLFWCursorPosCallback);
#ifdef __EMSCRIPTEN__
            glfwSetMouseButtonCallback(window, EmscriptenActivateMouseOnGLFWMouseButtonCallback);
#else
            glfwSetMouseButtonCallback(window, OnGLFWMouseButtonCallback);
#endif
        }
}

