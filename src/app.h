#pragma once

#include <chrono>
#include <GLFW/glfw3.h>
#include <entt/entity/registry.hpp>

namespace Chemical {

    class App {

    public:
        App();
        ~App();
        void Loop();

        GLFWwindow* window;
        entt::registry registry;

        using clock = std::chrono::high_resolution_clock;
        double delta_time = 0.0;
        clock::time_point old_time;
    };
}
