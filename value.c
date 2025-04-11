#include "value.h"
#include "memory.h"
#include <stdio.h>

void init_value_array(ValueArray *array) {
  array->used = 0;
  array->capacity = 0;
  array->values = NULL;
}

void write_value_array(ValueArray *array, Value value) {
  if (array->capacity < array->used + 1) {
    size_t current_capacity = array->capacity;
    array->capacity = COMPUTE_ARRAY_CAPACITY(current_capacity);
    array->values =
        GROW_ARRAY(Value, array->values, current_capacity, array->capacity);
  }
  array->values[array->used] = value;
  array->used++;
}

void free_value_array(ValueArray *array) {
  FREE_ARRAY(Value, array->values, array->capacity);
  init_value_array(array);
}

void print_value(Value value) { printf("%g", value); }
