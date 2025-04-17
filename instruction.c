#include "instruction.h"
#include "memory.h"

void init_instructions_array(InstructionsArray *array) {
  array->used = 0;
  array->capacity = 0;
  array->values = NULL;
  array->line_numbers = NULL;
}

void free_instructions_array(InstructionsArray *array) {
  FREE_ARRAY(uint8_t, array->values, array->capacity);
  FREE_ARRAY(size_t, array->line_numbers, array->capacity);
  init_instructions_array(array);
}

void write_instructions_array(InstructionsArray *array, uint8_t value,
                              size_t line_number) {
  if (array->capacity < array->used + 1) {
    size_t current_capacity = array->capacity;
    array->capacity = COMPUTE_ARRAY_CAPACITY(current_capacity);
    array->values =
        GROW_ARRAY(uint8_t, array->values, current_capacity, array->capacity);
    array->line_numbers = GROW_ARRAY(size_t, array->line_numbers,
                                     current_capacity, array->capacity);
  }
  array->values[array->used] = value;
  array->line_numbers[array->used] = line_number;
  array->used++;
}
