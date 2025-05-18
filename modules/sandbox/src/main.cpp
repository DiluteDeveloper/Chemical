
// this include is for testing only
#include <../../chemical/src/graphics/shader_program.h>
// this include is for testing only
#include <../../chemical/src/resources.h>
#include <chemical/core.h>
#include <chemical/graphics/mesh.h>
#include <chemical/graphics/misc.h>
#include <chemical/window.h>
using namespace Chemical;

int main() {
  InitialiseChemical();

  std::optional<Window> window = CreateNewWindow("Chemical 1.1.2", 1280, 720);

  if (!window.has_value())
    return -1;

  Graphics::RGBColour background(30, 60, 85);

  Graphics::SetBackgroundColour(background);
  //
  // const std::vector<float> vertices = {
  // 	-0.5f, -0.5f,
  // 	0.0f, 0.5f,
  // 	0.5f, -0.5f
  // };
  // const std::vector<unsigned int> indices = {
  // 	0,1,2
  // };

  Graphics::StaticMesh2D mesh =
      Graphics::CreateStaticMesh2D(Graphics::Shape2D::TRIANGLE);

  Chemical::Graphics::ShaderSourceFiles shader_source;
  shader_source.vs_file_path =
      std::string(Chemical::GetResourceDirectory() + "/shaders/test_shader.vs");
  shader_source.fs_file_path =
      std::string(Chemical::GetResourceDirectory() + "/shaders/test_shader.fs");

  Chemical::Graphics::ShaderProgram program(shader_source);

  if (program.GetStatus() == 0)
    return -1;
  program.Bind();

  program.SetUniform3F("colour", 0.5f, 0.8f, 0.9f);

  while (!WindowShouldClose(window.value())) {
    PollEvents();

    Graphics::ClearColourBuffer();

    mesh.Draw();

    Graphics::SwapBuffers(window.value());
  }
  TerminateChemical();
}
