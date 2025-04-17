#include "constant.h"
#include "memory.h"

void init_constants_array(ConstantsArray *array) {
  array->used = 0;
  array->capacity = 0;
  array->values = NULL;
}

void free_constants_array(ConstantsArray *array) {
  FREE_ARRAY(Constant, array->values, array->capacity);
  init_constants_array(array);
}

void write_constants_array(ConstantsArray *array, Constant value) {
  if (array->capacity < array->used + 1) {
    size_t current_capacity = array->capacity;
    array->capacity = COMPUTE_ARRAY_CAPACITY(current_capacity);
    array->values =
        GROW_ARRAY(Constant, array->values, current_capacity, array->capacity);
  }
  array->values[array->used] = value;
  array->used++;
}
