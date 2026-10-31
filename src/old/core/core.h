#pragma once

#include "input.h"
#include "rendering/opengl/gl_renderer.h"

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

    std::unique_ptr<OpenGL::Renderer> renderer;
    std::unique_ptr<Input> input;

    Window window;

    void ConfigureSpdlog() const;
  };
} // namespace Chemical
