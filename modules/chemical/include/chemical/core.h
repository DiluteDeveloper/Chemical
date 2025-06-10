#pragma once

#include "chemical/graphics/opengl/renderer.h"
#include "chemical/input.h"
#include "chemical/window.h"

#include <glm/glm.hpp>
#include <memory>

namespace Chemical {

  // Represents the entire Chemical Engine
  // NOT INTENDED to be accessible at all by user scripts
  class Core {
  public:
    Core(const std::string_view& window_title, const glm::vec2& window_size);
    ~Core();

    void StartGameLoop();

    void SetBackgroundColour(const glm::vec3& colour);

    // These assets should NOT be passed to any object
    // that has a lifetime exceeding the lifetime of Core;
    // These objects have the same lifetime as Core

    std::unique_ptr<GL_Renderer> renderer;
    std::unique_ptr<Input> input;
    std::unique_ptr<Scene> active_scene;

    Window window;

    void ConfigureSpdlog() const;
  };
} // namespace Chemical
