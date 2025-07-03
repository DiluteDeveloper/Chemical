#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "rendering/material.hpp"
#include "rendering/shader.hpp"
#include "util/transform.hpp"
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "spdlog/spdlog.h"

#include "rendering/model_loader.hpp"
#include "util/camera.hpp"

#include "rendering/cube.hpp"
#include "window.hpp"

using namespace Chemical;

int main() {
  spdlog::set_pattern("%^[%s] [%!] [%#] %$%v");

  GLFWwindow *window = Window::InitialiseGLContextAndGLFWWindow(
      "Game Development 0.15.0", 1080, 720);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.3, 0.3, 0.6, 1.0);

  ShaderTraits traits;
  std::ifstream file("/mnt/storage/Chemical/Chemical/modules/chemical/res/"
                     "shaders/LitShader.vs");

  if (!file) {
    SPDLOG_ERROR(R"(Failed to read file "" : returning 0)");
  }

  traits.vs_source = std::string((std::istreambuf_iterator<char>(file)),
                                 std::istreambuf_iterator<char>());

  file.close();

  std::ifstream file2("/mnt/storage/Chemical/Chemical/modules/chemical/res/"
                      "shaders/LitShader.fs");

  if (!file2) {
    SPDLOG_ERROR(R"(Failed to read file "" : returning 0)");
  }

  traits.fs_source = std::string((std::istreambuf_iterator<char>(file2)),
                                 std::istreambuf_iterator<char>());

  file2.close();
  Shader shader(traits);
  if (shader.compile_status == -1)
    throw std::runtime_error("Shader failed to compile");

  shader.Bind();

  glm::mat4 proj = glm::perspective(90.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);
  shader.SetUniformMatrix4FV("v_proj", 1, GL_FALSE, &proj[0][0]);

  glm::mat4 modelmat = glm::mat4(1.0f);
  // modelmat = glm::rotate(modelmat, glm::radians(-90.0f), glm::vec3(1, 0, 0));
  shader.SetUniformMatrix4FV("v_model", 1, GL_FALSE, &modelmat[0][0]);
  CameraController camera(window);

  // Material material;
  // material.ambient = glm::vec3(0.2, 0.2, 0.2);
  // material.diffuse = glm::vec3(1.0, 0.3, 0.9);
  // material.specular = glm::vec3(1.0, 1.0, 1.0);
  // material.shininess = 32;

  std::optional<Model> model_opt =
      ModelLoader::LoadModel("res/models/store.fbx");
  if (!model_opt)
    throw std::runtime_error("failed to load model");

  Model &model = model_opt.value();

  shader.SetUniform3F("material.ambient", model.material.ambient.x,
                      model.material.ambient.y, model.material.ambient.z);
  shader.SetUniform3F("material.diffuse", model.material.diffuse.x,
                      model.material.diffuse.y, model.material.diffuse.z);
  shader.SetUniform3F("material.specular", model.material.specular.x,
                      model.material.specular.y, model.material.specular.z);
  shader.SetUniform1UI("material.shininess", model.material.shininess);

  unsigned int vao = 0, vbo = 0, ibo = 0;
  glCreateVertexArrays(1, &vao);
  glCreateBuffers(1, &vbo);
  glCreateBuffers(1, &ibo);

  glNamedBufferStorage(vbo, sizeof(float) * model.mesh.vertices.size() * 6,
                       &model.mesh.vertices[0], GL_DYNAMIC_STORAGE_BIT);
  glNamedBufferStorage(ibo, sizeof(unsigned int) * model.mesh.indices.size(),
                       &model.mesh.indices[0], GL_DYNAMIC_STORAGE_BIT);

  glVertexArrayVertexBuffer(vao, 0, vbo, 0, 6 * sizeof(float));
  glVertexArrayElementBuffer(vao, ibo);

  glVertexArrayAttribBinding(vao, 0, 0);
  glVertexArrayAttribBinding(vao, 1, 0);

  glEnableVertexArrayAttrib(vao, 0);
  glEnableVertexArrayAttrib(vao, 1);

  glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));

  glBindVertexArray(vao);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  bool cursor_disabled = true;

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.Update(window);
    shader.SetUniformMatrix4FV(
        "v_view", 1, GL_FALSE,
        &glm::inverse(camera.transform.ToMatrix())[0][0]);
    shader.SetUniform3FV("viewPos", 1, &camera.transform.position[0]);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
      if (cursor_disabled)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      else
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      cursor_disabled = !cursor_disabled;
    }

    glDrawElements(GL_TRIANGLES, model.mesh.indices.size(), GL_UNSIGNED_INT,
                   nullptr);

    glfwPollEvents();

    glfwSwapBuffers(window);
  }

  Window::DestroyGLFWWindow(window);
  return 0;
}
