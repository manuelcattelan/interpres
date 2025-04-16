#ifndef interpres_constant_h
#define interpres_constant_h

#include <stdlib.h>

/* This typedef abstracts how the language constants are actually represented in
 * C, allowing us to change that representation without changing the rest of the
 * existing code that passes around variables of this type. */
typedef double Constant;
/* A series of constants is defined as a dynamic array which holds two counters:
 * "capacity" and "used", which represent respectively the number of elements in
 * the array and the number of elements that are actually in use. In this case,
 * the actual values of the array represent the constants that each instruction
 * inside the chunk will need for its execution. */
typedef struct {
  size_t used;
  size_t capacity;
  Constant *values;
} ConstantsArray;

/*
 * @brief Initialize a new array of constants.
 * We set a starting value of 0 for "used" and "capacity", while the constants
 * array starts completely empty; we do not even allocate space for a raw array
 * during initialization.
 *
 * @param array A pointer to the array to initialize
 * @return void
 */
void init_constants_array(ConstantsArray *array);
/*
 * @brief Append a new value to the constants array.
 * If the constants array still has free space left, this function will simply
 * push the new value to it. Otherwise, it will allocate additional space for
 * the new value by copying the old array to a new one, growing it's size and
 * finally appending the new value to the just created array.
 *
 * @param array A pointer to the array to append the new value to
 * @param value The value to append to the array
 * @return void
 */
void write_constants_array(ConstantsArray *array, Constant value);
/*
 * @brief Free the constants array.
 * This function will free the memory allocated for the constants array and
 * re-initialize the array to an empty state by calling init_constants_array.
 *
 * @param array A pointer to the array to free
 * @return void
 */
void free_constants_array(ConstantsArray *array);

#endif
