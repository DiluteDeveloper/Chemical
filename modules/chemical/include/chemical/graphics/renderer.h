#pragma once

#include "chemical/resource_manager.h"
#include "chemical/scene.h"

namespace Chemical {
  namespace Graphics {

    class Renderer {
    public:
      Renderer(ResourceManager &resources);

      void RenderScene(const Scene &scene);

    protected:
      ResourceManager &resources;
    };
  } // namespace Graphics
} // namespace Chemical
