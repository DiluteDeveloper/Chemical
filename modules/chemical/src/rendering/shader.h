#pragma once

#include <stddef.h>
typedef struct CL_Uniform {
  unsigned int uniformLocation;
  char uniformName[50];
} CL_Uniform;

typedef struct CL_Shader {
  unsigned int rendererID;
  CL_Uniform uniforms[50];
  size_t count;

} CL_Shader;

// rendererID is 0 for failed creation
extern CL_Shader CL_CreateShader(const char* vsSource, const char* fsSource, const char* gsSource,
                                 const char* csSource);

extern void CL_BindShader(unsigned int rendererID);
extern void CL_DeleteShader(CL_Shader* shader);
