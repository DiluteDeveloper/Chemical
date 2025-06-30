#include "background.h"
#include "core/core.h"
#include "player.h"

using namespace Chemical;

int main() {
  Core core("Chemical 1.2.6_int2", glm::vec2(1280, 720));

  core.SetBackgroundColour(glm::vec3(50, 100, 50));

  // core.active_scene->MountSceneScript<Background>(&core);
  // core.active_scene->MountSceneScript<Player>(&core);

  core.StartGameLoop();
  return 0;
}
