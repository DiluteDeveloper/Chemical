#include "input.h"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"

namespace Chemical {

    void Input::OnGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));
        for(auto [_, cb] : input->key_callbacks) {
            cb(key, scancode, action, mods);
        }
    }
    void Input::OnGLFWCursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
        Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));
        for(auto [_, cb] : input->cursor_pos_callbacks) {
            cb(xpos, ypos);
        }
    }
    void Input::OnGLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));
        for(auto [_, cb] : input->mouse_button_callbacks) {
            cb(button, action, mods);
        }
    }
    void Input::OnGLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));
        for(auto [_, cb] : input->scroll_callbacks) {
            cb(xoffset, yoffset);
        }
    }

    bool Input::GetKey(int key) {
        return glfwGetKey(window, key);
    }

    void Input::SubscribeToKeyCallback(const KeyCallback cb, void* identifier) {
        auto [it, inserted] = key_callbacks.emplace(identifier, cb);
        if(!inserted)
            spdlog::error("Failed to add input callback: identifier is not unique");
    }
    void Input::UnsubscribeFromKeyCallback(void* identifier) {
        key_callbacks.erase(identifier); 
    }

    void Input::SubscribeToCursorPosCallback(const CursorPosCallback cb, void* identifier) {
        auto [it, inserted] = cursor_pos_callbacks.emplace(identifier, cb);
        if(!inserted)
            spdlog::error("Failed to add input callback: identifier is not unique");
    }
    void Input::UnsubscribeFromCursorPosCallback(void* identifier) {
        cursor_pos_callbacks.erase(identifier); 
    }

    void Input::SubscribeToMouseButtonCallback(const MouseButtonCallback cb, void* identifier) {
        auto [it, inserted] = mouse_button_callbacks.emplace(identifier, cb);
        if(!inserted)
            spdlog::error("Failed to add input callback: identifier is not unique");
    }
    void Input::UnsubscribeFromMouseButtonCallback(void* identifier) {
        mouse_button_callbacks.erase(identifier); 
    }
    void Input::SubscribeToScrollCallback(const ScrollCallback cb, void* identifier) {
        auto [it, inserted] = scroll_callbacks.emplace(identifier, cb);
        if(!inserted)
            spdlog::error("Failed to add input callback: identifier is not unique");
    }
    void Input::UnsubscribeFromScrollCallback(void* identifier) {
        scroll_callbacks.erase(identifier); 
    }
#ifdef __EMSCRIPTEN__
    void Input::EmscriptenActivateMouseOnGLFWMouseButtonCallback(GLFWwindow*, int, int, int) {
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

    Input::Input(GLFWwindow* window) : window(window) {

        glfwSetWindowUserPointer(window, this);
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

