#pragma once

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

    void SetGameLoopCallback(const std::function<void(Core &)> &callback);
    void StartGameLoop();

    Scene *GetActiveScene();

    void SetBackgroundColour(const glm::vec3 &colour);

    bool doTerminate = false;

  private:
    std::unique_ptr<Graphics::Renderer> renderer;
    std::unique_ptr<Scene> active_scene;

    Window window;

    std::function<void(Core &)> game_loop_callback;

    void Terminate();

    void ConfigureSpdlog() const;
  };
} // namespace Chemical
