
#include "read_file.h"

#include <stdio.h>
#include <stdlib.h>

char* CL_ReadEntireFile_Alloc(const char* filePath) {

  FILE* filePtr = fopen(filePath, "r");
  if (filePtr == NULL) {
    printf(R"(Failed to read file "%s")", filePath);
    return NULL;
  }
  fseek(filePtr, 0L, SEEK_END);
  size_t fileLen = ftell(filePtr);
  rewind(filePtr);

  char* str = malloc(fileLen + 1); // + 1 for null terminator
  str[fileLen] = '\0';

  size_t i = 0;
  while (i <= fileLen - 1) {
    str[i] = fgetc(filePtr);
    i++;
  }

  fclose(filePtr);

  return str;
}
