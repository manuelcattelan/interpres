#include "memory.h"
#include <stdlib.h>

void *reallocate_array(void *array, size_t old_capacity, size_t new_capacity) {
  if (new_capacity == 0) {
    free(array);
    return NULL;
  }
  void *new_array = realloc(array, new_capacity);
  if (new_array == NULL)
    exit(1);
  return new_array;
}
