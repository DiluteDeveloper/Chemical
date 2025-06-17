#include "core/window.h"

#include <stdio.h>

int main() {

  GLFWwindow* window = CMCL_CreateWindow(720, 480, "Chemical");
  CMCL_InitializeGLAD();

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }
}
