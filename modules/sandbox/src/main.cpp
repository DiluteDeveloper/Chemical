#include <chemical/core.h>
#include <chemical/graphics/misc.h>
#include <chemical/graphics/renderer.h>
#include <chemical/graphics/static_material.h>
#include <chemical/window.h>
using namespace Chemical;

int main() {
  InitialiseChemical();

  {
    std::optional<Window> window = CreateNewWindow("Chemical 1.1.7", 1280, 720);

    if (!window.has_value())
      return -1;

    Graphics::RGBColour background(30, 60, 85);

    Graphics::SetBackgroundColour(background);

    Graphics::Material mat1(glm::vec3(200, 100, 100));

    Graphics::StaticMeshTraits mesh_traits(Graphics::Shape::SQUARE);
    Graphics::StaticMeshTraits mesh_traits_2(Graphics::Shape::TRIANGLE, "default", "mat1");
    Graphics::StaticMeshTraits mesh_traits_3(Graphics::Shape::SQUARE);
    mesh_traits_3.transform.position.x = -0.5f;
    mesh_traits_3.transform.position.y = 0.5f;
    mesh_traits_3.transform.scale = glm::vec2(0.5f, 0.5f);

    Graphics::Renderer renderer;

    Graphics::Material &m = renderer.RegisterMaterial(mat1, "mat1");

    renderer.RegisterStaticMesh(mesh_traits_3);
    auto dyn = renderer.RegisterDynamicMesh(mesh_traits, "dyn_mesh");
    if (!dyn.has_value())
      return -1;
    auto meshid = renderer.RegisterDynamicMesh(mesh_traits_2, "mymesh");
    if (!meshid.has_value())
      return -1;
    auto mesh = renderer.GetDynamicMesh(meshid.value());
    mesh->transform.position.x = -0.5f;
    mesh->transform.scale = glm::vec2(0.1f, 0.1f);
    bool square = true;
    int i = 0;
    while (!WindowShouldClose(window.value())) {
      PollEvents();

      i++;

      m.albedo.r = std::sin((i / 100.0f)) * 255;
      m.albedo.g = std::sin(((i + 100) / 100.0f)) * 255;
      m.albedo.b = std::sin(((i + 200) / 100.0f)) * 255;

      mesh->transform.position.x = std::cos(i / 10.0f);
      mesh->transform.position.y = std::sin(i / 10.0f);

      if (i % 50 == 0) {
        renderer.DeregisterDynamicMesh("dyn_mesh");
        if (square) {
          dyn = renderer.RegisterDynamicMesh(mesh_traits_2, "dyn_mesh");
          if (!dyn.has_value())
            return -1;

          square = false;
        } else {
          dyn = renderer.RegisterDynamicMesh(mesh_traits, "dyn_mesh");
          if (!dyn.has_value())
            return -1;
          square = true;
        }
      }
      Graphics::ClearColourBuffer();

      renderer.Render();

      Graphics::SwapBuffers(window.value());
    }
  }
  TerminateChemical();
  return 0;
}
