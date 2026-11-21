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
        for(auto [_, cb] : key_callbacks) {
            cb(key, scancode, action, mods);
        }
    }
    void OnGLFWCursorPosCallback(GLFWwindow*, double xpos, double ypos) {
        for(auto [_, cb] : cursor_pos_callbacks) {
            cb(xpos, ypos);
        }
    }
    void OnGLFWMouseButtonCallback(GLFWwindow*, int button, int action, int mods) {

        for(auto [_, cb] : mouse_button_callbacks) {
            cb(button, action, mods);
        }
    }
    void OnGLFWScrollCallback(GLFWwindow*, double xoffset, double yoffset) {

        for(auto [_, cb] : scroll_callbacks) {
            cb(xoffset, yoffset);
        }
    }
}
namespace Chemical::Input {

        bool GetKey(int key) {
            return glfwGetKey(window, key);
        }

        void SubscribeToKeyCallback(const KeyCallback cb, void* identifier) {
            auto [it, inserted] = key_callbacks.emplace(identifier, cb);
            if(!inserted)
                spdlog::error("Failed to add input callback: identifier is not unique");
        }
        void UnsubscribeFromKeyCallback(void* identifier) {
            key_callbacks.erase(identifier); 
        }

        void SubscribeToCursorPosCallback(const CursorPosCallback cb, void* identifier) {
            auto [it, inserted] = cursor_pos_callbacks.emplace(identifier, cb);
            if(!inserted)
                spdlog::error("Failed to add input callback: identifier is not unique");
        }
        void UnsubscribeFromCursorPosCallback(void* identifier) {
            cursor_pos_callbacks.erase(identifier); 
        }

        void SubscribeToMouseButtonCallback(const MouseButtonCallback cb, void* identifier) {
            auto [it, inserted] = mouse_button_callbacks.emplace(identifier, cb);
            if(!inserted)
                spdlog::error("Failed to add input callback: identifier is not unique");
        }
        void UnsubscribeFromMouseButtonCallback(void* identifier) {
            mouse_button_callbacks.erase(identifier); 
        }
        void SubscribeToScrollCallback(const ScrollCallback cb, void* identifier) {
            auto [it, inserted] = scroll_callbacks.emplace(identifier, cb);
            if(!inserted)
                spdlog::error("Failed to add input callback: identifier is not unique");
        }
        void UnsubscribeFromScrollCallback(void* identifier) {
            scroll_callbacks.erase(identifier); 
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

