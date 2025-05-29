#include "chemical/core.h"
#include "chemical/resources.h"

using namespace Chemical;

void GameLoop(Core &core) {

  Scene &scene = core.GetScene();
  scene.GetTransform("right")->rotation += 0.6f;
}

int main() {
  Core core;
  Scene &scene = core.GetScene();

  core.SetGameLoopCallback(GameLoop);

  core.SetBackgroundColour(glm::vec3(50, 100, 50));

  Graphics::TextureTraits texture(GetResourceDirectory("textures/funny.png"));
  scene.CreateTexture(texture, "funny_texture");

  Graphics::Material *mat1 = scene.CreateMaterial("mat1");
  mat1->albedo = glm::vec3(255, 255, 255);
  mat1->texture_id = "funny_texture";

  Graphics::StaticMeshTraits square(Graphics::Shape::SQUARE);
  square.material_id = "mat1";
  square.transform_id = "left";
  Graphics::StaticMeshTraits triangle(Graphics::Shape::TRIANGLE);
  triangle.material_id = "mat1";
  triangle.transform_id = "right";

  Transform *left = scene.CreateTransform("left");
  left->position.x = -0.5f;
  Transform *right = scene.CreateTransform("right");
  right->position.x = 0.5f;

  scene.CreateStaticMesh(square);
  scene.CreateStaticMesh(triangle);

  core.StartGameLoop();
  return 0;
}
