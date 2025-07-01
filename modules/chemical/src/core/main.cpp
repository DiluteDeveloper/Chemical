#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "rendering/shader.hpp"
#include <fstream>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "spdlog/spdlog.h"

#include "rendering/cube.hpp"
#include "window.hpp"

using namespace Chemical;

int main() {
  spdlog::set_pattern("%^[%s] [%!] [%#] %$%v");

  GLFWwindow *window = Window::InitialiseGLContextAndGLFWWindow(
      "Game Development 0.11.0", 1080, 720);

  glClearColor(1.0, 1.0, 0.5, 1.0);

  ShaderTraits traits;
  std::ifstream file("/mnt/storage/Chemical/Chemical/modules/chemical/res/"
                     "shaders/default_shader.vs");

  if (!file) {
    SPDLOG_ERROR(R"(Failed to read file "" : returning 0)");
  }

  traits.vs_source = std::string((std::istreambuf_iterator<char>(file)),
                                 std::istreambuf_iterator<char>());

  file.close();

  std::ifstream file2("/mnt/storage/Chemical/Chemical/modules/chemical/res/"
                      "shaders/default_shader.fs");

  if (!file2) {
    SPDLOG_ERROR(R"(Failed to read file "" : returning 0)");
  }

  traits.fs_source = std::string((std::istreambuf_iterator<char>(file2)),
                                 std::istreambuf_iterator<char>());

  file2.close();
  Shader shader(traits);
  shader.Bind();

  std::vector<float> vertices = GetCubeVertices();
  std::vector<unsigned int> indices = GetCubeIndices();

  unsigned int vao = 0, vbo = 0, ibo = 0;
  glCreateVertexArrays(1, &vao);
  glCreateBuffers(1, &vbo);
  glCreateBuffers(1, &ibo);

  glNamedBufferStorage(vbo, sizeof(float) * vertices.size(), &vertices[0],
                       GL_DYNAMIC_STORAGE_BIT);
  glNamedBufferStorage(ibo, sizeof(unsigned int) * indices.size(), &indices[0],
                       GL_DYNAMIC_STORAGE_BIT);

  glVertexArrayVertexBuffer(vao, 0, vbo, 0, 3 * sizeof(float));
  glVertexArrayElementBuffer(vao, ibo);

  glVertexArrayAttribBinding(vao, 0, 0);

  glEnableVertexArrayAttrib(vao, 0);

  glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);

  glBindVertexArray(vao);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

    glfwPollEvents();

    glfwSwapBuffers(window);
  }

  Window::DestroyGLFWWindow(window);
  return 0;
}
