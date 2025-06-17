#pragma once

#include <stdio.h>

#include <GLFW/glfw3.h>
#include <GLAD/glad.h>

extern GLFWwindow* CMCL_CreateWindow(int width, int height, const char* title);

extern void CMCL_InitializeGLAD();