#include "background.h"

#include "chemical/scene.h"
#include "chemical/util/image.h"
#include "spdlog/spdlog.h"

using namespace Chemical;

void Background::BeginScript() {
  SPDLOG_INFO("Begun background script");

  std::optional<Util::Image> background_image =
      Util::LoadImage(scene->GetResourcePath("textures/sky.png"), 3);

  auto [bg_texture, _2] = scene->CreateTexture("background", background_image.value());
  scene->CreateMaterial("background", "background");

  scene->CreateTransform("background", glm::vec2(0.0f), 0);

  scene->CreateStaticSprite("default", "background", "background", glm::vec2(128.0f, 72.0f));
}
void Background::FrameUpdate() {
  // SPDLOG_INFO("Update background script");
}
void Background::EndScript() {
  SPDLOG_INFO("End background script");
}
