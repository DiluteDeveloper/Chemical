#pragma once

#include "util/safe_ptr.h"

namespace Chemical {

  class Core;
  class Scene;
  class Input;
  // To be inherited from
  class SceneScript {
  public:
    SceneScript(SafePtr<Core> core);
    virtual ~SceneScript() = default;

    virtual void BeginScript() = 0;
    virtual void FrameUpdate() = 0;
    virtual void EndScript() = 0;

  protected:
    SafePtr<Scene> scene;
    SafePtr<Input> input;
  };
} // namespace Chemical
