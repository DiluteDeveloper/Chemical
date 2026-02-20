#pragma once

#include <glm/glm.hpp>

namespace Chemical {
  namespace GUI {

    struct RenderingSettings {
      float orbit_trail_width = 1.0f;
      int orbit_trail_length = 5000.0f;
      glm::fvec3 background_colour = glm::fvec3(0.2f, 0.2f, 0.3f);

      glm::fvec3 body_a_colour = glm::fvec3(1.0f, 1.0f, 0.0f);
      glm::fvec3 body_b_colour = glm::fvec3(0.0f, 1.0f, 1.0f);
      glm::fvec3 trail_a_colour = glm::fvec3(1.0f, 0.0f, 1.0f);
      glm::fvec3 trail_b_colour = glm::fvec3(0.5f, 1.0f, 0.5f);
    };

    extern RenderingSettings
    RenderRenderingMenu(const RenderingSettings &settings);
  } // namespace GUI
} // namespace Chemical
