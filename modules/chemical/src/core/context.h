#pragma once

#include "glfw/glfw3.h"
#include "maths/vec.h"

typedef struct CL_ContextTraits {
  unsigned int windowWidth;
  unsigned int windowHeight;
  const char* windowTitle;

  unsigned int glVersionMajor;
  unsigned int glVersionMinor;

  unsigned int swapInterval;

  CL_FVec3 clearColour;
} CL_ContextTraits;
extern GLFWwindow* CL_CreateContext(const CL_ContextTraits* traits);
