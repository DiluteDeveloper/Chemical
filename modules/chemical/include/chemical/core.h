#pragma once

#include "graphics/scene_renderer.h"

#include <functional>
#include <memory>

class GLFWwindow;
namespace Chemical {

  using Window = GLFWwindow *;
  class Core {
  public:
    Core();
    ~Core();

    void SetGameLoopCallback(const std::function<void(Core &)> &callback);
    void StartGameLoop();

    Scene &GetScene();

    void SetBackgroundColour(const glm::vec3 &colour);

  private:
    std::unique_ptr<Graphics::SceneRenderer> renderer;
    std::unique_ptr<Scene> scene;

    Window window;

    std::function<void(Core &)> game_loop_callback;
  };
} // namespace Chemical
