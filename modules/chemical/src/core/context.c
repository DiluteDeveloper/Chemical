#include "context.h"

#include "glad/glad.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

extern void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                   GLchar const* message, void const* user_param);

GLFWwindow* CL_CreateContext(const CL_ContextTraits* traits) {
  assert(traits->glVersionMajor > 0);
  assert(traits->glVersionMinor > 0);
  assert(traits->windowWidth > 0);
  assert(traits->windowHeight > 0);
  assert(traits->clearColour.x >= 0 && traits->clearColour.x <= 1);
  assert(traits->clearColour.y >= 0 && traits->clearColour.y <= 1);
  assert(traits->clearColour.z >= 0 && traits->clearColour.z <= 1);

  // GLFW INITIALIZATION ----------------------------

  if (!glfwInit()) {
    printf("GLFW initialization failed.\n");
  } else
    printf("GLFW initialization successful.\n");

  // GLFW INITIALIZATION ----------------------------

  // GLFW WINDOW HINTS ----------------------------------

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, traits->glVersionMajor);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, traits->glVersionMinor);

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // GLFW WINDOW HINTS ----------------------------------

  // GLFW WINDOW SETUP -------------------------------------------------

  GLFWwindow* window =
      glfwCreateWindow(traits->windowWidth, traits->windowHeight, traits->windowTitle, NULL, NULL);

  if (!window) {
    printf("Creating GLFW window failed.\n");
  } else
    printf("GLFW window successfully created.\n");

  // GLFW WINDOW SETUP -------------------------------------------------

  glfwSwapInterval(traits->swapInterval);

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("GLAD functions failed to load.\n");
  } else
    printf("GLAD functions successfully loaded.\n");

  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

  glDebugMessageCallback(&DebugCallback, NULL);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);

  glClearColor(traits->clearColour.x, traits->clearColour.y, traits->clearColour.z, 1.0f);

  return window;
}

// prevents message duplication
int prevMessageID = -1;

// OpenGL debug callback
void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                            GLchar const* message, void const* user_param) {

  if ((GLuint)prevMessageID == id)
    return;
  prevMessageID = id;

  const char* source_str = "";
  switch (source) {
  case GL_DEBUG_SOURCE_API:
    source_str = "API";
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    source_str = "WINDOW SYSTEM";
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    source_str = "SHADER COMPILER";
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    source_str = "THIRD PARTY";
  case GL_DEBUG_SOURCE_APPLICATION:
    source_str = "APPLICATION";
  case GL_DEBUG_SOURCE_OTHER:
    source_str = "OTHER";
  default:
    break;
  }

  const char* type_str = "";
  switch (type) {
  case GL_DEBUG_TYPE_ERROR:
    type_str = "ERROR";
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    type_str = "DEPRECATED_BEHAVIOR";
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    type_str = "UNDEFINED_BEHAVIOR";
  case GL_DEBUG_TYPE_PORTABILITY:
    type_str = "PORTABILITY";
  case GL_DEBUG_TYPE_PERFORMANCE:
    type_str = "PERFORMANCE";
  case GL_DEBUG_TYPE_MARKER:
    type_str = "MARKER";
  case GL_DEBUG_TYPE_OTHER:
    type_str = "OTHER";
  default:
    break;
  }

  switch (severity) {
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    printf("OpenGL Debug Notification: %s, %s, %i: %s\n", source_str, type_str, (int)id, (char*)message);
    break;
  case GL_DEBUG_SEVERITY_LOW:
    printf("OpenGL Debug Low Warning: %s, %s, %i: %s\n", source_str, type_str, (int)id, (char*)message);
    break;
  case GL_DEBUG_SEVERITY_MEDIUM:
    printf("OpenGL Debug Medium Warning: %s, %s, %i: %s\n", source_str, type_str, (int)id, (char*)message);
    break;
  case GL_DEBUG_SEVERITY_HIGH:
    printf("OpenGL Debug Error: %s, %s, %i: %s\n", source_str, type_str, (int)id, (char*)message);
    break;
  default:
    break;
  }
}
