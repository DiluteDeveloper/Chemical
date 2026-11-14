#include "input.h"
#include "GLFW/glfw3.h"


namespace Chemical {

        std::unordered_map<void*, Input::KeyCallback> Input::key_callbacks;
        std::unordered_map<void*, Input::CursorPosCallback> Input::cursor_pos_callbacks;
        GLFWwindow* Input::window;

        void Input::Initialise(GLFWwindow* window) {
            Input::window = window;

            glfwSetKeyCallback(window, Input::OnGLFWKeyCallback);
            glfwSetCursorPosCallback(window, Input::OnGLFWCursorPosCallback);
        }
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

        void Input::OnGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
#ifdef __EMSCRIPTEN__
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif
            for(auto [data, cb] : key_callbacks) {
                cb(key, scancode, action, mods, data);
            }
        }
        void Input::OnGLFWCursorPosCallback(GLFWwindow*, double xpos, double ypos) {

            for(auto [data, cb] : cursor_pos_callbacks) {
                cb(xpos, ypos, data);
            }
        }
}

