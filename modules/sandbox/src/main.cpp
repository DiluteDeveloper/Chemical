#include "chemical/core.h"
#include "chemical/graphics/shader.h"
#include "chemical/util/image.h"
#include "spdlog/spdlog.h"

#include <algorithm>

using namespace Chemical;

glm::vec2 velocity = glm::vec2(0);
int GameLoop(Core &core) {
  Scene *scene = core.GetActiveScene();

  scene->GetTransform("right")->rotation += 0.6f;

  Transform *left = scene->GetTransform("left");

  Input *input = core.GetInput();

  if (input->GetKey(Input::Key::SPACE) == Input::KeyState::PRESS && left->position.y <= -0.69f) {
    velocity.y = 0.04f;
  }
  if (input->GetKey(Input::Key::D) == Input::KeyState::PRESS) {
    velocity.x += 0.001f;
  } else if (input->GetKey(Input::Key::A) == Input::KeyState::PRESS) {
    velocity.x -= 0.002f;
  } else
    velocity.x *= 0.8f;

  velocity.x = std::min(velocity.x, 0.03f);
  velocity.x = std::max(velocity.x, -0.03f);
  if (left->position.y <= -0.7f)
    left->position.y = -0.7f;
  else
    velocity.y -= 0.001f;
  left->position += velocity;

  return 0;
}

int StartGame(Core &core) {

  Scene *scene = core.GetActiveScene();

  core.SetBackgroundColour(glm::vec3(50, 100, 50));

  std::optional<Util::Image> image = Util::LoadImage(scene->GetResourcePath("textures/funny.png"));

  auto [texture, _1] = scene->CreateTexture("funny", image.value());

  std::optional<Util::Image> background_image = Util::LoadImage(scene->GetResourcePath("textures/xp.jpg"));

  auto [bg_texture, _2] = scene->CreateTexture("background", background_image.value());
  scene->CreateMaterial("background", "background");
  scene->CreateMaterial("default", "funny");

  scene->CreateTransform("left", glm::vec2(-0.5f, 0.0f), 0, glm::vec2(0.3f, 0.7f));
  scene->CreateTransform("right", glm::vec2(0.5f, 0.0f), 0, glm::vec2(0.6f, 0.6f));
  scene->CreateTransform("background", glm::vec2(0.0f), 0, glm::vec2(2.0f, 2.0f));

  Graphics::StaticMeshTraits square(Graphics::Shape::SQUARE);
  square.material_id = "default";
  square.transform_id = "left";
  Graphics::StaticMeshTraits triangle(Graphics::Shape::TRIANGLE);
  triangle.material_id = "default";
  triangle.transform_id = "right";

  Graphics::StaticMeshTraits background_mesh(Graphics::Shape::SQUARE);
  background_mesh.material_id = "background";
  background_mesh.transform_id = "background";

  Graphics::ShaderTraits st;
  st.vs_file_path = scene->GetResourcePath("shaders/test_shader.vs");
  st.fs_file_path = scene->GetResourcePath("shaders/test_shader_2.fs");

  Graphics::Shader shader(st);

  scene->MoveConstructShader("test_shader", std::move(shader));
  Graphics::ShaderTraits st2;
  st2.vs_file_path = scene->GetResourcePath("shaders/test_shader.vs");
  st2.fs_file_path = scene->GetResourcePath("shaders/test_shader.fs");

  Graphics::Shader shader2(st2);

  scene->MoveConstructShader("default2", std::move(shader2));

  scene->CreateStaticMesh("default2", background_mesh);
  scene->CreateStaticMesh("default", square);
  scene->CreateStaticMesh("test_shader", triangle);

  return 0;
}

int main() {
  Core core;

  core.SetGameLoopCallback(GameLoop);

  if (StartGame(core) == -1)
    return -1;

  core.StartGameLoop();
  return 0;
}
