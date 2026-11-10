#include "glfw.h"
#include "spdlog/spdlog.h"
#include <stdexcept>

namespace Chemical {

    GLFW::GLFW() {

        glfwSetErrorCallback([](int code, const char* message) {
                spdlog::error("GLFW error: {} - {}", code, message);
        });

        // Wayland is used by default but only X11 works on WSL
#ifdef FORCE_X11
        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif

        glfwInit();

        if(!glfwInit())
            throw std::runtime_error("Failed to initialise GLFW!");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(1280, 720, "ChemicalMC_PK", nullptr, nullptr);

        if(!window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window!");
        }
    }
    GLFW::~GLFW() {

        glfwDestroyWindow(window);
        glfwTerminate();
    }

}
