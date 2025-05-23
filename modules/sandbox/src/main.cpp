
#include <chemical/core.h>
#include <chemical/graphics/misc.h>
#include <chemical/graphics/renderer.h>
#include <chemical/window.h>
using namespace Chemical;

int main() {
  InitialiseChemical();

  std::optional<Window> window = CreateNewWindow("Chemical 1.1.3", 1280, 720);

  if (!window.has_value())
    return -1;

  Graphics::RGBColour background(30, 60, 85);

  Graphics::SetBackgroundColour(background);

  Graphics::StaticMeshTraits mesh_traits(Graphics::Shape::TRIANGLE, 1);
  Graphics::StaticMeshTraits mesh_traits_2(Graphics::Shape::SQUARE);

  Graphics::Renderer renderer;

  Graphics::Shader *default_shader = renderer.GetShader(renderer.GetDefaultShaderID());

  if (default_shader != nullptr) {
    default_shader->SetUniform3F("colour", 0.5f, 0.8f, 0.9f);
  }

  renderer.RegisterDynamicMesh(mesh_traits);
  renderer.RegisterStaticMesh(mesh_traits_2);

  while (!WindowShouldClose(window.value())) {
    PollEvents();

    Graphics::ClearColourBuffer();

    renderer.Render();

    Graphics::SwapBuffers(window.value());
  }
  TerminateChemical();
  return 0;
}
