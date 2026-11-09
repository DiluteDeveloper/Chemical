#include "glfw.h"
#include <stdexcept>

namespace Chemical {

    GLFWwindow* GLFW::initialise_glfw() {

        // Wayland is used by default but only X11 works on WSL
#ifdef FORCE_X11
        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif

        glfwInit();

        if(!glfwInit())
            throw std::runtime_error("Failed to initialise GLFW!");

        // glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        GLFWwindow* window = glfwCreateWindow(1280, 720, "ChemicalMC_PK", nullptr, nullptr);

        if(!window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window!");
        }
        return window;
    }
    void GLFW::terminate_glfw(GLFWwindow* window) {

        glfwDestroyWindow(window);
        glfwTerminate();
    }
}
