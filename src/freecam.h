#pragma once

#include <webgpu/webgpu_cpp.h>
#include <entt/entity/registry.hpp>

namespace Chemical {

    class Freecam {
    public:
        static void Initialise(entt::registry& registry);

        static void Update(entt::registry& registry, double delta_time);
        static void WriteViewMatrix(wgpu::Queue queue, wgpu::Buffer view_buffer);

        static float speed;
        static float sensitivity;

    };
}
