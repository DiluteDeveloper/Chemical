#include "chemical/scene_script.h"

#include "chemical/core.h"

namespace Chemical {

  SceneScript::SceneScript(SafePtr<Core> core) : input(core->input.get()), scene(core->active_scene.get()) {}

} // namespace Chemical
