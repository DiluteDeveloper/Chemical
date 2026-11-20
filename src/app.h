#pragma once

#include <GLFW/glfw3.h>
#include <entt/entity/registry.hpp>
#include "renderers/renderer.h"

namespace Chemical {

    class App {

    public:
        static void Start();
        GLFWwindow* window;
        Renderer renderer;

        entt::registry registry;

        static void ApplicationLoop(void* arg);

    };
}
