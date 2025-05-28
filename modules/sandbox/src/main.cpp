#include "chemical/core.h"
#include "chemical/graphics/material.h"
#include "chemical/graphics/texture.h"
#include "chemical/resources.h"

using namespace Chemical;

void GameLoop(Core &core) { core.GetRenderer().Render(); }

int main() {
  Chemical::Core core;

  core.SetGameLoopCallback(GameLoop);

  Graphics::Renderer &renderer = core.GetRenderer();

  core.SetBackgroundColour(glm::vec3(50, 100, 50));

  Graphics::TextureTraits texture(GetResourceDirectory("textures/funny.png"));

  Graphics::Material mat1;
  mat1.albedo = glm::vec3(200, 10, 10);
  mat1.texture_id = "funny";

  Graphics::StaticMeshTraits mesh(Graphics::Shape::SQUARE);
  mesh.transform.position.x -= 0.5f;
  mesh.transform.scale *= 0.5f;
  mesh.material_id = "mat1";
  Graphics::StaticMeshTraits mesh2(Graphics::Shape::TRIANGLE);

  renderer.RegisterTexture(texture, "funny");
  renderer.RegisterMaterial(mat1, "mat1");

  renderer.RegisterStaticMesh(mesh);
  renderer.RegisterStaticMesh(mesh2);

  core.StartGameLoop();
  return 0;
}
