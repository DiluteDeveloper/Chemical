#include "core/context.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"

int main() {

  CL_ContextTraits traits;
  traits.glVersionMajor = 4;
  traits.glVersionMinor = 6;
  traits.windowHeight = 720;
  traits.windowWidth = 1280;
  traits.windowTitle = "Chemical";
  traits.clearColour.x = 0.3f;
  traits.clearColour.y = 0.7f;
  traits.clearColour.z = 1.0f;

  GLFWwindow* window = CL_CreateContext(&traits);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }
}
