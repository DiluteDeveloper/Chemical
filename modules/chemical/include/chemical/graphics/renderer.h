#pragma once

#include "chemical/scene.h"

namespace Chemical {
  namespace Graphics {

    // Responsible for managing data that is exclusively used
    // for rendering and performing rendering processes
    class Renderer {
    public:
      // Depends on resource manager for persistent rendering data
      Renderer() = default;

      // Renders a scene frame
      void RenderScene(Scene &scene);

    protected:
    };
  } // namespace Graphics
} // namespace Chemical
