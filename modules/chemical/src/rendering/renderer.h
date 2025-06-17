#pragma once

typedef struct CL_ShaderArray {
  CL_Shader* shaders;
  size_t count;
  size_t allocSize;
} CL_ShaderArray;

typedef struct CL_Renderer {
  CL_ShaderArray shaderArray;
  unsigned int bufferlessVAO;
} CL_Renderer;
