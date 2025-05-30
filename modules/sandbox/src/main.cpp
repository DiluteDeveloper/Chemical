#include "chemical/core.h"
#include "chemical/resource_manager.h"

using namespace Chemical;

void GameLoop(Core &core) {

  Scene *scene = core.GetActiveScene();
  scene->GetTransform("right")->rotation += 0.6f;
}

int main() {
  Core core;
  Scene *scene = core.GetActiveScene();
  ResourceManager &resources = core.GetResourceManager();

  core.SetGameLoopCallback(GameLoop);

  core.SetBackgroundColour(glm::vec3(50, 100, 50));

  Graphics::TextureTraits texture(resources.GetResourceFilePath("textures/funny.png"));
  resources.LoadTexture(texture, "funny_texture");

  Graphics::Material *mat1 = resources.LoadMaterial("mat1");
  mat1->albedo = glm::vec3(255, 255, 255);
  mat1->texture_id = "funny_texture";

  Transform *left = scene->CreateTransform("left");
  left->position.x = -0.5f;
  Transform *right = scene->CreateTransform("right");
  right->position.x = 0.5f;

  Graphics::StaticMeshTraits square(Graphics::Shape::SQUARE);
  square.material_id = "mat1";
  square.transform_id = "left";
  Graphics::StaticMeshTraits triangle(Graphics::Shape::TRIANGLE);
  triangle.material_id = "mat1";
  triangle.transform_id = "right";
  triangle.shader_id = "test_shader";

  Graphics::ShaderTraits st;
  st.vs_file_path = resources.GetResourceFilePath("shaders/test_shader.vs");
  st.fs_file_path = resources.GetResourceFilePath("shaders/test_shader_2.fs");

  resources.LoadShader(st, "test_shader");

  scene->CreateStaticMesh(square);
  scene->CreateStaticMesh(triangle);

  core.StartGameLoop();
  return 0;
}
