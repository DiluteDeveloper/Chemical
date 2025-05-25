#include <chemical/core.h>
#include <chemical/graphics/misc.h>
#include <chemical/graphics/renderer.h>
#include <chemical/graphics/static_material.h>
#include <chemical/window.h>
using namespace Chemical;

int main() {
  InitialiseChemical();

  {
    std::optional<Window> window = CreateNewWindow("Chemical 1.1.5", 1280, 720);

    if (!window.has_value())
      return -1;

    Graphics::RGBColour background(30, 60, 85);

    Graphics::SetBackgroundColour(background);

    Graphics::Material mat1(glm::vec3(200, 100, 100));
    Graphics::Material def(glm::vec3(255, 255, 255));

    Graphics::StaticMeshTraits mesh_traits(Graphics::Shape::SQUARE, "default", "mat1");
    Graphics::StaticMeshTraits mesh_traits_2(Graphics::Shape::TRIANGLE, "default", "mat1");

    Graphics::Renderer renderer;

    // Graphics::Shader *default_shader = renderer.GetShader("default");
    //
    // if (default_shader != nullptr) {
    //   default_shader->SetUniform3F("colour", 0.5f, 0.8f, 0.9f);
    // }

    Graphics::Material &m = renderer.RegisterMaterial(mat1, "mat1");

    renderer.RegisterMaterial(def, "default");

    auto dyn = renderer.RegisterDynamicMesh(mesh_traits, "dyn_mesh");
    if (!dyn.has_value())
      return -1;

    bool square = true;
    int i = 0;
    while (!WindowShouldClose(window.value())) {
      PollEvents();

      i++;

      m.albedo.r = std::sin((i / 100.0f)) * 255;
      m.albedo.g = std::sin(((i + 100) / 100.0f)) * 255;
      m.albedo.b = std::sin(((i + 200) / 100.0f)) * 255;

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
