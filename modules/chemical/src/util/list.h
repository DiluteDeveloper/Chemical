#pragma once

#include <stddef.h>

typedef struct CL_List_Alloc {
  char* data;

  size_t numElements;

  // Size of allocation / elementSize
  size_t allocSize;
  const size_t elementSize;
} CL_List_Alloc;

extern CL_List_Alloc CL_CreateList(size_t elementSize, size_t defaultCapacity);
extern void* CL_GetListElement(CL_List_Alloc* list, size_t index);
extern void CL_AddListElementBack(CL_List_Alloc* list, void* element);
extern void CL_DeleteList(CL_List_Alloc* list);
