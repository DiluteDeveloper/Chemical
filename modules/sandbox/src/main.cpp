#include "chemical/core.h"
#include "chemical/graphics/shader.h"
#include "chemical/util/image.h"

#include <algorithm>

using namespace Chemical;

glm::vec2 velocity = glm::vec2(0);
float rotspeed = 00.0f;
int GameLoop(Core &core) {
  Scene *scene = core.GetActiveScene();

  scene->GetTransform("right")->rotation += 0.6f;

  Transform *left = scene->GetTransform("left");

  Input *input = core.GetInput();

  if (input->GetKey(Input::Key::SPACE) == Input::KeyState::PRESS && left->position.y <= -0.69f) {
    velocity.y = 0.08f;
    left->position.y += 0.1f;
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
  if (left->position.y <= -0.7f && std::abs(left->position.x) < 12.8f) {

    left->position.y = -0.7f;

    float rot = ((int(std::abs(left->rotation) + 360) % 360) / 90.0f) - 1.8f;
    // SPDLOG_INFO("{}", rot);
    if (velocity.y != 0)
      velocity.y = (std::abs(velocity.y)) + (std::abs(rot) * 0.04f) * glm::sign(rot);
  } else {
    velocity.y -= 0.0005f;
    rotspeed -= velocity.x * 20;
    rotspeed -= ((std::abs(velocity.y * 1.4f)) * glm::sign(velocity.x));
    left->rotation += rotspeed;
  }
  rotspeed = std::max(rotspeed, -3.0f);
  rotspeed = std::min(rotspeed, 3.0f);

  left->position += velocity;

  Camera *camera = scene->GetCamera("default");
  Transform *cam_transform = scene->GetTransform(camera->transform_id);
  cam_transform->position = scene->GetTransform("left")->position;

  if (input->GetKey(Input::Key::Z) == Input::KeyState::PRESS) {
    camera->SetZoomLevel(camera->GetZoomLevel() - 6.0f);
  }
  if (input->GetKey(Input::Key::X) == Input::KeyState::PRESS) {
    camera->SetZoomLevel(camera->GetZoomLevel() + 6.0f);
  }
  return 0;
}

int StartGame(Core &core) {

  Scene *scene = core.GetActiveScene();

  core.SetBackgroundColour(glm::vec3(50, 100, 50));

  std::optional<Util::Image> image = Util::LoadImage(scene->GetResourcePath("textures/white.png"), 3);

  auto [texture, _1] = scene->CreateTexture("funny", image.value());

  std::optional<Util::Image> background_image =
      Util::LoadImage(scene->GetResourcePath("textures/sky.png"), 3);

  auto [bg_texture, _2] = scene->CreateTexture("background", background_image.value());
  scene->CreateMaterial("background", "background");
  scene->CreateMaterial("default", "funny");

  scene->CreateTransform("left", glm::vec2(-0.5f, -0.7f), 0);
  scene->CreateTransform("right", glm::vec2(0.5f, 0.0f), 0);
  scene->CreateTransform("background", glm::vec2(0.0f), 0);

  Graphics::ShaderTraits st2;
  st2.vs_file_path = scene->GetResourcePath("shaders/test_shader.vs");
  st2.fs_file_path = scene->GetResourcePath("shaders/test_shader.fs");

  Graphics::Shader shader2(st2);

  scene->MoveConstructShader("default2", std::move(shader2));

  scene->CreateStaticSprite("default2", "background", "background", glm::vec2(128.0f, 72.0f));
  scene->CreateStaticSprite("test_shader", "right", "default");

  scene->CreateStaticSprite("default", "left", "default", glm::vec2(1.0f, 2.0f));

  // scene->GetCamera("default")->SetZoomLevel(-2000.0f);

  return 0;
}

int main() {
  Core core("Chemical 1.2.3", glm::vec2(1280, 720));

  core.SetGameLoopCallback(GameLoop);

  if (StartGame(core) == -1)
    return -1;

  core.StartGameLoop();
  return 0;
}
