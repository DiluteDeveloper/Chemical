#pragma once

#include "chemical/input.h"
#include "chemical/window.h"
#include "graphics/renderer.h"

#include <functional>
#include <memory>

namespace Chemical {

  // Represents the entire Chemical Engine
  class Core {
  public:
    Core();
    ~Core();

    void SetGameLoopCallback(const std::function<int(Core &)> &callback);
    void StartGameLoop();

    Scene *GetActiveScene();
    Input *GetInput();

    void SetBackgroundColour(const glm::vec3 &colour);

  private:
    std::unique_ptr<Graphics::Renderer> renderer;
    std::unique_ptr<Scene> active_scene;
    std::unique_ptr<Input> input;

    Window window;

    std::function<int(Core &)> game_loop_callback;

    void ConfigureSpdlog() const;
  };
} // namespace Chemical
