#include "memory.h"

#include <stdlib.h>

void *reallocate_instructions_array(void *instructions, size_t old_capacity,
                                    size_t new_capacity) {
  if (new_capacity == 0) {
    free(instructions);
    return NULL;
  }
  void *new_instructions_array = realloc(instructions, new_capacity);
  if (new_instructions_array == NULL)
    exit(1);
  return new_instructions_array;
}
