#pragma once

#include "graphics/renderer.h"

#include <functional>

class GLFWwindow;
namespace Chemical {

  using Window = GLFWwindow *;
  class Core {
  public:
    Core();
    ~Core();

    void SetGameLoopCallback(const std::function<void(Core &)> &callback);
    void StartGameLoop();

    Graphics::Renderer &GetRenderer();

    void SetBackgroundColour(const glm::vec3 &colour);

  private:
    std::unique_ptr<Graphics::Renderer> renderer;

    Window window;

    std::function<void(Core &)> game_loop_callback;
  };
} // namespace Chemical
