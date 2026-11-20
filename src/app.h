#pragma once

#include <GLFW/glfw3.h>
#include <entt/entity/registry.hpp>

namespace Chemical {

    class App {

    public:
        static void Start();
        GLFWwindow* window;

        entt::registry registry;

        static void ApplicationLoop(void* arg);

    };
}
