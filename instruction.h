#ifndef interpres_instructions_h
#define interpres_instructions_h

#include <stdint.h>
#include <stdlib.h>

/* A series of instructions is defined as a dynamic array which holds two
 * counters: "capacity" and "used", which represent respectively the number of
 * elements in the array and the number of elements that are actually in use.
 * In this case, the actual values of the array represent the set of
 * instructions we're dealing with, while the "line_numbers" array is used to
 * keep track of the number of the line in which each instruction appears. For
 * this reason, these two arrays will always grow and shrink in parallel. */
typedef struct {
  size_t used;
  size_t capacity;
  size_t *line_numbers;
  uint8_t *values;
} InstructionsArray;

/*
 * @brief Initialize a new array of instructions.
 * We set a starting value of 0 for "used" and "capacity", while the
 * instructions array starts off completely empty; we do not even allocate space
 * for a raw array of instructions during initialization.
 *
 * @param array A pointer to the instructions array to initialize
 * @return void
 */
void init_instructions_array(InstructionsArray *array);
/*
 * @brief Free the instructions array.
 * This function will free the memory allocated for the instructions array and
 * re-initialize it to an empty state by calling init_instructions_array.
 *
 * @param array A pointer to the instructions array to free
 * @return void
 */
void free_instructions_array(InstructionsArray *array);
/*
 * @brief Append a new instruction to the instructions array.
 * If the instructions array still has free space left, this function will
 * simply push the new instruction to it. Otherwise, it will allocate additional
 * space for the new instruction by copying the old array to a new one, growing
 * it's size and finally appending the new instruction to the just created
 * instructions array.
 *
 * @param array A pointer to the instructions array to append the new
 * instruction to
 * @param instruction The instruction to append to the array
 * @param line_number The line number of the instruction to append to the
 * instructions array
 * @return void
 */
void write_instructions_array(InstructionsArray *array, uint8_t instruction,
                              size_t line_number);

#endif
