#include <unordered_map>

#include "input.h"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"

namespace {

    using namespace Chemical;
    std::unordered_map<void*, Input::KeyCallback> key_callbacks;
    std::unordered_map<void*, Input::CursorPosCallback> cursor_pos_callbacks;
    std::unordered_map<void*, Input::MouseButtonCallback> mouse_button_callbacks;
    std::unordered_map<void*, Input::ScrollCallback> scroll_callbacks;
    GLFWwindow* window;

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
    void OnGLFWMouseButtonCallback(GLFWwindow*, int button, int action, int mods) {

        for(auto [data, cb] : mouse_button_callbacks) {
            cb(button, action, mods, data);
        }
    }
    void OnGLFWScrollCallback(GLFWwindow*, double xoffset, double yoffset) {

        for(auto [data, cb] : scroll_callbacks) {
            cb(xoffset, yoffset, data);
        }
    }
}
namespace Chemical::Input {

        bool GetKey(int key) {
            return glfwGetKey(window, key);
        }

        void SubscribeToKeyCallback(const KeyCallback cb, void* data) {
            if(data == nullptr) 
                data = (void*)&cb;
            auto [it, inserted] = key_callbacks.emplace(data, cb);
            if(!inserted)
                spdlog::error("Failed to add callback: data pointer conflicts with existing callback subscriber data pointer!");
        }
        void UnsubscribeFromKeyCallback(void* data_ptr) {
            key_callbacks.erase(data_ptr); 
        }

        void SubscribeToCursorPosCallback(const CursorPosCallback cb, void* data) {
            if(data == nullptr) 
                data = (void*)&cb;
            auto [it, inserted] = cursor_pos_callbacks.emplace(data, cb);
            if(!inserted)
                spdlog::error("Failed to add callback: data pointer conflicts with existing callback subscriber data pointer!");
        }
        void UnsubscribeFromCursorPosCallback(void* data_ptr) {
            cursor_pos_callbacks.erase(data_ptr); 
        }

        void SubscribeToMouseButtonCallback(const MouseButtonCallback cb, void* data) {
            if(data == nullptr) 
                data = (void*)&cb;
            auto [it, inserted] = mouse_button_callbacks.emplace(data, cb);
            if(!inserted)
                spdlog::error("Failed to add callback: data pointer conflicts with existing callback subscriber data pointer!");
        }
        void UnsubscribeFromMouseButtonCallback(void* data_ptr) {
            mouse_button_callbacks.erase(data_ptr); 
        }
        void SubscribeToScrollCallback(const ScrollCallback cb, void* data) {
            if(data == nullptr) 
                data = (void*)&cb;
            auto [it, inserted] = scroll_callbacks.emplace(data, cb);
            if(!inserted)
                spdlog::error("Failed to add callback: data pointer conflicts with existing callback subscriber data pointer!");
        }
        void UnsubscribeFromScrollCallback(void* data_ptr) {
            scroll_callbacks.erase(data_ptr); 
        }
#ifdef __EMSCRIPTEN__
        void EmscriptenActivateMouseOnGLFWMouseButtonCallback(GLFWwindow*, int, int, int) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetMouseButtonCallback(window, OnGLFWMouseButtonCallback);
        }
#endif

        void SetCursorDisabled() {
    #ifdef WSL
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
#else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    #endif
        } 
        void SetCursorEnabled() {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        void Initialise(GLFWwindow* _window) {
            window = _window;

            glfwSetKeyCallback(window, OnGLFWKeyCallback);
            glfwSetCursorPosCallback(window, OnGLFWCursorPosCallback);
            glfwSetScrollCallback(window, OnGLFWScrollCallback);
#ifdef __EMSCRIPTEN__
            glfwSetMouseButtonCallback(window, EmscriptenActivateMouseOnGLFWMouseButtonCallback);
#else
            glfwSetMouseButtonCallback(window, OnGLFWMouseButtonCallback);
#endif
        }
}

