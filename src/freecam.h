#pragma once

#include <webgpu/webgpu_cpp.h>

#include "physics/physics.h"

namespace Chemical {

    class Freecam {
    public:
        static void Initialise(PhysicsBody& _body);

        static void Update(double delta_time);
        static void WriteViewMatrix(wgpu::Queue queue, wgpu::Buffer view_buffer);

        static float speed;
#ifdef WSL
        static float sensitivity;
#else
        float sensitivity = 0.2;
#endif

    };
}
