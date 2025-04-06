#ifndef interpres_chunk_h
#define interpres_chunk_h

#include "common.h"

/* Each instruction in bytecode format has a one-byte operation code that
 * represents what kind of operation we're dealing with, e.g. adding,
 * subtracting, looking up a variable, etc... */
typedef enum {
  OP_RETURN,
} OpCode;

/* Here is the standard implementation of a dynamic array in C. Besides the
 * array of bytes that will hold our instructions, we keep track of the size of
 * the array we have allocated, i.e. the number of elements in the array,
 * "capacity", and the number of allocated entries that are actually in use.
 * "used". */
typedef struct {
  size_t used;
  size_t capacity;
  uint8_t *instructions;
} Chunk;

/*
 * @brief Initialize a new chunk of instructions.
 * The chunk's instructions array starts completely empty, we do not even
 * allocate space for a raw array during initialization.
 *
 * @param chunk A pointer to the chunk to initialize
 * @return void
 */
void init_chunk(Chunk *chunk);
/*
 * @brief Append a new instruction to the chunk's instructions array.
 * If the chunk's instructions array still has free space left, this function
 * will simply push the new instruction to the chunk's instructions. Otherwise,
 * it will allocate additional space for the new instruction by copying the old
 * instructions array to a new chunk, doubling it's size and finally appending
 * the new instruction to the new array.
 *
 * @param chunk A pointer to the chunk to append the new instruction to
 * @param instruction The instruction to append to the chunk
 * @return void
 */
void write_chunk(Chunk *chunk, uint8_t byte);

#endif
