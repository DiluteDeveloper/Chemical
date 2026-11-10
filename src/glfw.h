#pragma once

#include <webgpu/webgpu_cpp.h>
#include <GLFW/glfw3.h>

namespace Chemical {

    class GLFW {
    public:
        GLFW();
        ~GLFW();

        GLFWwindow* window;

    };
}
