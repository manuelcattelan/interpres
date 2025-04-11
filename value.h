#ifndef interpres_value_h
#define interpres_value_h

#include "common.h"

/* This typedef abstracts how the language values are actually represented in C,
 * allowing us to change that representation without changing the rest of the
 * existing code that passes around variables of this type. */
typedef double Value;
/* A ValueArray is implemented as a dynamic array which holds two counters:
 * "capacity" and "used", the number of elements in the array and the number of
 * elements that are actually in use. Inside a ValueArray, we also store the
 * actual items of the array. */
typedef struct {
  size_t used;
  size_t capacity;
  Value *values;
} ValueArray;

/*
 * @brief Initialize a new array of values.
 * We set a starting value of 0 for "used" and "capacity", while the values
 * array starts completely empty; we do not even allocate space for a raw array
 * during initialization.
 *
 * @param array A pointer to the array to initialize
 * @return void
 */
void init_value_array(ValueArray *array);
/*
 * @brief Append a new value to the values array.
 * If the values array still has free space left, this function will simply push
 * the new value to the values array. Otherwise, it will allocate additional
 * space for the new value by copying the old values array to a new array,
 * growing it's size and finally appending the new value to the new array.
 *
 * @param array A pointer to the array to append the new value to
 * @param value The value to append to the array
 * @return void
 */
void write_value_array(ValueArray *array, Value value);
/*
 * @brief Free the values array.
 * This function will free the memory allocated for the values array and
 * re-initialize the array to an empty state by calling init_value_array.
 *
 * @param array A pointer to the array to free
 * @return void
 */
void free_value_array(ValueArray *array);
/*
 * @brief Print a value to the standard output.
 * This function will print the Value given as parameter to the standard output;
 * it is primarily used for debugging values stored in a chunk's value array.
 *
 * @param value The value to print
 * @return void
 */
void print_value(Value value);

#endif
