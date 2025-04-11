#include "memory.h"
#include <stdlib.h>

void *reallocate_array(void *current_array, size_t current_capacity,
                       size_t new_capacity) {
  if (new_capacity == 0) {
    free(current_array);
    return NULL;
  }
  void *new_array = realloc(current_array, new_capacity);
  if (new_array == NULL)
    exit(EXIT_FAILURE);
  return new_array;
}
