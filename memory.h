#ifndef interpres_memory_h
#define interpres_memory_h

#include "common.h"

/* Here we define a heuristic value for the growth operation of a dynamic array.
 * A value of 8 means that 8 will be the minimum number of values that we will
 * allocate space for when we need to grow the array. */
#define GROWTH_THRESHOLD 8
/* Here we define the multiplier we will use to extend the capacity of a dynamic
 * array when we need to grow it. A value of 2 means that we will double the
 * size of the array every time we need to grow it. */
#define GROWTH_FACTOR 2

/*
 * @brief Compute the new capacity of a dynamic array before growing it.
 * This macro will return the capacity of the dynamic array that has to be
 * allocated in case the current one does not fit any new items.
 *
 * @param current_capacity The current capacity of the dynamic array
 * @return The new capacity of the dynamic array
 */
#define COMPUTE_ARRAY_CAPACITY(current_capacity)                               \
  ((current_capacity) < GROWTH_THRESHOLD ? GROWTH_THRESHOLD                    \
                                         : (current_capacity) * GROWTH_FACTOR)
/*
 * @brief Wrapper around reallocate_array function that pretties up getting the
 * size of the dynamic array's type and casting the resulting void* back to
 * the pointer of the correct type.
 *
 * @param array_type The type of the dynamic array's values
 * @param array The dynamic array to reallocate
 * @param current_capacity The current capacity of the dynamic array
 * @param new_capacity The new capacity of the dynamic array
 * @return The reallocated dynamic array
 */
#define GROW_ARRAY(array_type, array, current_capacity, new_capacity)          \
  (array_type *)reallocate_array(array, sizeof(array_type) * current_capacity, \
                                 sizeof(array_type) * new_capacity)
/*
 * @brief Wrapper around reallocate_array function that pretties up freeing the
 * dynamic array and re-initializing it to an empty state.
 *
 * @param array_type The type of the dynamic array's values
 * @param array The dynamic array to free
 * @param current_capacity The current capacity of the dynamic array
 * @return void
 */
#define FREE_ARRAY(array_type, array, current_capacity)                        \
  reallocate_array(array, sizeof(array_type) * current_capacity, 0)
/*
 * @brief Reallocate a dynamic array to a new size.
 * This function will allocate a new dynamic array of the given size and copy
 * the old array to the new one. If the new size is 0, it will free the old
 * array and return NULL.
 *
 * @param array The dynamic array to reallocate
 * @param current_capacity The current capacity of the dynamic array
 * @param new_capacity The new capacity of the dynamic array
 * @return The reallocated dynamic array or NULL
 */
void *reallocate_array(void *current_array, size_t current_capacity,
                       size_t new_capacity);

#endif
