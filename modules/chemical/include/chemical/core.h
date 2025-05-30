#pragma once

#include "chemical/resource_manager.h"
#include "graphics/renderer.h"

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

    Scene *GetActiveScene();
    ResourceManager &GetResourceManager();

    void SetBackgroundColour(const glm::vec3 &colour);

  private:
    std::unique_ptr<Graphics::Renderer> renderer;
    std::unique_ptr<Scene> active_scene;
    std::unique_ptr<ResourceManager> resources;

    Window window;

    std::function<void(Core &)> game_loop_callback;
  };
} // namespace Chemical
