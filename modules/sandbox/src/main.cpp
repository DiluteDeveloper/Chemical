#include "chemical/core.h"
#include "chemical/graphics/shader.h"
#include "chemical/util/image.h"
#include "spdlog/spdlog.h"

#include <tuple>

using namespace Chemical;

void GameLoop(Core &core) {
  Scene *scene = core.GetActiveScene();
  scene->GetTransform("right")->rotation += 0.6f;
}

int main() {
  Core core;
  Scene *scene = core.GetActiveScene();

  core.SetGameLoopCallback(GameLoop);

  core.SetBackgroundColour(glm::vec3(50, 100, 50));

  std::optional<Util::Image> image = Util::LoadImage(scene->GetResourcePath("textures/funny.png"));

  if (!image.has_value()) {
    SPDLOG_ERROR("Failed to load image.");
    return -1;
  }
  auto [texture, inserted] = scene->CreateTexture("funny", image.value());
  if (!inserted)
    return -1;

  scene->CreateMaterial("default", "funny");

  scene->CreateTransform("left", glm::vec2(-0.5f, 0.0f));
  scene->CreateTransform("right", glm::vec2(0.5f, 0.0f));

  Graphics::StaticMeshTraits square(Graphics::Shape::SQUARE);
  square.material_id = "default";
  square.transform_id = "left";
  Graphics::StaticMeshTraits triangle(Graphics::Shape::TRIANGLE);
  triangle.material_id = "default";
  triangle.transform_id = "right";

  Graphics::ShaderTraits st;
  st.vs_file_path = scene->GetResourcePath("shaders/test_shader.vs");
  st.fs_file_path = scene->GetResourcePath("shaders/test_shader_2.fs");

  Graphics::Shader shader(st);

  if (!scene->MoveConstructShader("test_shader", std::move(shader)))
    return -1;

  scene->CreateStaticMesh("default", square);
  scene->CreateStaticMesh("test_shader", triangle);

  core.StartGameLoop();
  return 0;
}
