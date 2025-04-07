#ifndef interpres_memory_h
#define interpres_memory_h

#include "common.h"

/* Here we define a heuristic value for the growth operation of the instructions
 * array. A value of 8 means that 8 will be the minimum number of instructions
 * that we will allocate space for when we need to grow the instructions array.
 */
#define GROWTH_THRESHOLD 8
/* Here we define the multiplier we will use to extend the capacity of
 * instructions array when we need to grow it. A value of 2 means that we will
 * double the size of the instructions array every time we need to grow it. */
#define GROWTH_FACTOR 2

/*
 * @brief Compute the new capacity of the instructions array before growing it.
 * This macro will return the capacity of the instructions array that has to be
 * allocated in case the current one does not fit any new instructions.
 *
 * @param instructions_array_capacity The current capacity of the instructions
 * array
 * @return The new capacity of the instructions array
 */
#define COMPUTE_INSTRUCTIONS_ARRAY_CAPACITY(instructions_array_capacity)       \
  ((instructions_array_capacity) < GROWTH_THRESHOLD                            \
       ? GROWTH_THRESHOLD                                                      \
       : (instructions_array_capacity) * GROWTH_FACTOR)
/*
 * @brief Wrapper around reallocate_instructions_array function that pretties
 * up getting the size of the instructions array's type and casting the
 * resulting void* back to the pointer of the correct type.
 *
 * @param instructions_type The type of the instructions array
 * @param instructions The instructions array to reallocate
 * @param old_capacity The old capacity of the instructions array
 * @param new_capacity The new capacity of the instructions array
 * @return The reallocated instructions array
 */
#define GROW_INSTRUCTIONS_ARRAY(instructions_type, instructions, old_capacity, \
                                new_capacity)                                  \
  (instructions_type *)reallocate_instructions_array(                          \
      instructions, sizeof(instructions_type) * (old_capacity),                \
      sizeof(instructions_type) * (new_capacity))
/*
 * @brief Wrapper around reallocate_instructions_array function that pretties
 * up freeing the instructions array and re-initializing it to an empty state.
 *
 * @param instructions_type The type of the instructions array
 * @param instructions The instructions array to reallocate
 * @param old_capacity The old capacity of the instructions array
 * @return void
 */
#define FREE_INSTRUCTIONS_ARRAY(instructions_type, instructions, old_capacity) \
  reallocate_instructions_array(instructions,                                  \
                                sizeof(*instructions) * old_capacity, 0)

void *reallocate_instructions_array(void *instructions, size_t old_capacity,
                                    size_t new_capacity);

#endif
