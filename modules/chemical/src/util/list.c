#include "list.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CL_List_Alloc CL_CreateList(size_t elementSize, size_t defaultCapacity) {
  assert(defaultCapacity > 0);

  CL_List_Alloc list = {.numElements = 0, .allocSize = defaultCapacity, .elementSize = elementSize};

  list.data = (char*)malloc(elementSize * defaultCapacity);

  return list;
}
void* CL_GetListElement(CL_List_Alloc* list, size_t index) {
  if (index >= list->allocSize) {
    printf("Out of bounds occurred.");
    return NULL;
  }
  return list->data + (index * list->elementSize);
}
void CL_AddListElementBack(CL_List_Alloc* list, void* element) {
  if (list->allocSize <= list->numElements) {
    list->allocSize *= 2;
    list->data = (char*)realloc(list->data, list->allocSize * list->elementSize);
  }
  memcpy(list->data + (list->numElements * list->elementSize), element, list->elementSize);
  list->numElements++;
}
void CL_DeleteList(CL_List_Alloc* list) {
  free(list->data);
}
