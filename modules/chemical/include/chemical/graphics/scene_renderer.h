#pragma once

#include "chemical/graphics/shader.h"
#include "chemical/scene.h"

namespace Chemical {
  namespace Graphics {

    class SceneRenderer {
    public:
      SceneRenderer();

      void RenderScene(const Scene &scene);

      bool RegisterShader(const ShaderTraits &traits, const ObjectID &id);

    protected:
      std::unordered_map<ObjectID, Shader> shaders;
    };
  } // namespace Graphics
} // namespace Chemical
