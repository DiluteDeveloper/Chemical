
#include "player.h"

#include "chemical/input.h"
#include "chemical/scene.h"
#include "spdlog/spdlog.h"

using namespace Chemical;

void Player::BeginScript() {
  SPDLOG_INFO("Begun player script");
}
void Player::FrameUpdate() {

  Graphics::Material *mat = scene->GetMaterial("default");
  mat->tint.r = std::sin(frame_idx / 100.0f) * 255.0f;
  mat->tint.g = std::sin(frame_idx / 100.0f + 100) * 255.0f;
  mat->tint.b = std::sin(frame_idx / 100.0f + 200) * 255.0f;

  Transform *player = scene->GetTransform("player");

  if (input->GetKey(Input::Key::SPACE) == Input::KeyState::PRESS && player->position.y <= -0.69f) {
    velocity.y = 0.08f;
    player->position.y += 0.1f;
    rotspeed += 0.3f;
  }
  if (input->GetKey(Input::Key::D) == Input::KeyState::PRESS) {
    velocity.x += 0.0005f;
  } else if (input->GetKey(Input::Key::A) == Input::KeyState::PRESS) {
    velocity.x -= 0.0005f;
  } else
    velocity.x *= 0.8f;

  velocity.x = std::min(velocity.x, 0.03f);
  velocity.x = std::max(velocity.x, -0.03f);
  if (player->position.y <= -0.7f && std::abs(player->position.x) < 12.8f) {

    player->position.y = -0.7f;

    float rot = ((int(std::abs(player->rotation) + 360) % 360) / 90.0f) - 1.8f;
    // SPDLOG_INFO("{}", rot);
    if (velocity.y != 0)
      velocity.y = (std::abs(velocity.y)) + (std::abs(rot) * 0.04f) * glm::sign(rot);
  } else {
    velocity.y -= 0.0005f;
    rotspeed -= velocity.x * 20;
    rotspeed -= ((std::abs(velocity.y * 1.4f)) * glm::sign(velocity.x));
    player->rotation += rotspeed;
  }
  rotspeed = std::max(rotspeed, -3.0f);
  rotspeed = std::min(rotspeed, 3.0f);

  player->position += velocity;

  Transform *cam_transform = scene->GetTransform("camera");
  Camera *camera = scene->GetCamera("default");
  cam_transform->position = player->position;

  if (input->GetKey(Input::Key::Z) == Input::KeyState::PRESS) {
    camera->SetZoomLevel(camera->GetZoomLevel() - 6.0f);
  }
  if (input->GetKey(Input::Key::X) == Input::KeyState::PRESS) {
    camera->SetZoomLevel(camera->GetZoomLevel() + 6.0f);
  }

  frame_idx++;
}
void Player::EndScript() {
  SPDLOG_INFO("End player script");
}
