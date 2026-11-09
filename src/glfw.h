#pragma once

#include <GLFW/glfw3.h>

namespace Chemical {

    class GLFW {
    public:
        // needs to be terminated wtih glfwTerminate();
        static GLFWwindow* initialise_glfw();
        static void terminate_glfw(GLFWwindow* window);
    };
}
