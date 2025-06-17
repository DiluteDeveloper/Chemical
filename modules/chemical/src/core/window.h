#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

extern GLFWwindow* CMCL_CreateWindow(int width, int height, const char* title);

extern void CMCL_InitializeGLAD();
