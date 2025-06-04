#pragma once

#include "chemical/scene_script.h"

#include <glm/glm.hpp>

class Player : public Chemical::SceneScript {
  glm::vec2 velocity = glm::vec2(0);
  float rotspeed = 0.0f;
  unsigned int frame_idx = 0;

  using Chemical::SceneScript::SceneScript;

  void BeginScript();
  void FrameUpdate();
  void EndScript();
};
