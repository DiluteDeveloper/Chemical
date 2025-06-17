#include "core/context.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "io/file.h"
#include "rendering/shader.h"

#include <stdio.h>
#include <stdlib.h>

// returns success
_Bool SetupShaders() {

  char* vsSource = CL_ReadEntireFile_Alloc(
      "/mnt/storage/Chemical/Chemical/modules/chemical/res/shaders/default_shader.vs");
  if (vsSource == NULL)
    return 0;
  char* fsSource = CL_ReadEntireFile_Alloc(
      "/mnt/storage/Chemical/Chemical/modules/chemical/res/shaders/default_shader.fs");
  if (fsSource == NULL) {
    free(vsSource);
    return 0;
  }

  CL_Shader shader = CL_CreateShader(vsSource, fsSource, NULL, NULL);
  if (shader.rendererID == 0) {
    free(vsSource);
    free(fsSource);
    return 0;
  } else {
    CL_BindShader(shader.rendererID);

    free(fsSource);
    free(vsSource);
    return 1;
  }
}

int main() {

  setlinebuf(stdout);

  CL_ContextTraits contextTraits = {
      .glVersionMajor = 4,
      .glVersionMinor = 6,
      .windowHeight = 720,
      .windowWidth = 1280,
      .windowTitle = "Chemical",
      .clearColour.x = 0.3f,
      .clearColour.y = 0.7f,
      .clearColour.z = 1.0f,
  };

  GLFWwindow* window = CL_CreateContext(&contextTraits);

  if (!SetupShaders()) {
    return -1;
  }

  unsigned int vArray = 0;
  glCreateVertexArrays(1, &vArray);
  glBindVertexArray(vArray);
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }
}
