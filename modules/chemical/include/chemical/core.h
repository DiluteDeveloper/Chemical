#pragma once

#include "chemical/input.h"
#include "chemical/window.h"
#include "graphics/renderer.h"

#include <memory>

namespace Chemical {

  // Represents the entire Chemical Engine
  // NOT INTENDED to be accessible at all by user scripts
  class Core {
  public:
    Core(const char *window_title, glm::vec2 window_size);
    ~Core();

    void StartGameLoop();

    void SetBackgroundColour(const glm::vec3 &colour);

    std::unique_ptr<Graphics::Renderer> renderer;
    std::unique_ptr<Scene> active_scene;
    std::unique_ptr<Input> input;

    Window window;

    void ConfigureSpdlog() const;
  };
} // namespace Chemical
