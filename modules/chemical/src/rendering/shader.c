#include "shader.h"

#include "glad/glad.h"

#include <assert.h>
#include <stdio.h>

// Returns 0 for failure
unsigned int CL_CompileShader(const char* source, GLenum type) {
  assert(type == 0x8DD9 || type == 0x8B31 || type == 0x8DD9 || type == 0x8B30);

  unsigned int shader = glCreateShader(type);

  glShaderSource(shader, 1, &source, NULL);

  glCompileShader(shader);

  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success != GL_TRUE) {

    char info_log[1024];
    glGetShaderInfoLog(shader, 1024, NULL, info_log);
    printf("Shader type %d failed to compile : %s\n", type, info_log);

    return 0;
  }

  return shader;
}

// Returns compilation/attach success for compiling and attaching shader source to program
_Bool TryCompileAttachShader(const unsigned int rendererID, const char* source, GLenum type) {
  unsigned int s = CL_CompileShader(source, type);
  if (s != 0) {
    glAttachShader(rendererID, s);
    glDeleteShader(s);
    return 1;
  } else {
    glDeleteShader(s);
    return 0;
  }
}

// Returns rendererID 0 for failed compilation, 1 for successful
void FinaliseShader(CL_Shader* shader) {
  glLinkProgram(shader->rendererID);

  GLint success = 0;
  glGetProgramiv(shader->rendererID, GL_LINK_STATUS, &success);
  if (success != GL_TRUE) {
    char infoLog[1024];
    glGetProgramInfoLog(shader->rendererID, 1024, NULL, infoLog);

    printf("Shader program failed to compile : %s\n", infoLog);
    glDeleteProgram(shader->rendererID);
    shader->rendererID = 0;
  }
  glValidateProgram(shader->rendererID);

  int uniform_count = 0;
  glGetProgramiv(shader->rendererID, GL_ACTIVE_UNIFORMS, &uniform_count);

  if (uniform_count > 0) {
    if (uniform_count > 50) {
      printf("MAX UNIFORMS REACHED!\n");
    } else {

      for (int i = 0; i < uniform_count; ++i) {
        CL_Uniform* u = &shader->uniforms[shader->count];
        shader->count++;
        glGetActiveUniform(shader->rendererID, i, 30, NULL, NULL, NULL, u->uniformName);
        u->uniformLocation = glGetUniformLocation(shader->rendererID, u->uniformName);
      }
    }
  }
}

CL_Shader CL_CreateShader(const char* vsSource, const char* fsSource, const char* gsSource,
                          const char* csSource) {
  assert(!(vsSource == NULL && fsSource == NULL && gsSource == NULL && csSource == NULL));

  _Bool compilationFailed = 0;

  CL_Shader shader = {.rendererID = glCreateProgram()};
  if (vsSource != NULL) {
    if (!TryCompileAttachShader(shader.rendererID, vsSource, GL_VERTEX_SHADER)) {
      compilationFailed = 1;
    }
  }
  if (fsSource != NULL) {
    if (!TryCompileAttachShader(shader.rendererID, fsSource, GL_FRAGMENT_SHADER)) {
      compilationFailed = 1;
    }
  }
  if (gsSource != NULL) {
    if (!TryCompileAttachShader(shader.rendererID, gsSource, GL_GEOMETRY_SHADER)) {
      compilationFailed = 1;
    }
  }
  if (csSource != NULL) {
    if (!TryCompileAttachShader(shader.rendererID, csSource, GL_COMPUTE_SHADER)) {
      compilationFailed = 1;
    }
  }

  if (compilationFailed)
    return shader;

  FinaliseShader(&shader);
  return shader;
}
void CL_BindShader(unsigned int rendererID) {
  glUseProgram(rendererID);
}
void CL_DeleteShader(CL_Shader* shader) {
  glDeleteProgram(shader->rendererID);
}
