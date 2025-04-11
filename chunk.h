#ifndef interpres_chunk_h
#define interpres_chunk_h

#include "common.h"
#include "value.h"

/* Each instruction in bytecode format has a one-byte operation code that
 * represents what kind of operation we're dealing with, e.g. adding,
 * subtracting, looking up a variable, etc... */
typedef enum {
  OP_RETURN,
  OP_NEGATE,
  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_CONSTANT,
} OpCode;
/* A Chunk is implemented as a dynamic array which holds two counters:
 * "capacity" and "used", the number of elements in the array and the number of
 * elements that are actually in use. Inside a Chunk, we also store the actual
 * array of values, which are in this case constants of the program we're
 * executing, the actual instructions of the program and the line number of
 * those instructions, associated between each other by index. */
typedef struct {
  size_t used;
  size_t capacity;
  size_t *instruction_lines;
  uint8_t *instructions;
  ValueArray constants;
} Chunk;

/*
 * @brief Initialize a new chunk of instructions.
 * We set a starting value of 0 for "used" and "capacity", while the chunk's
 * instructions array and values array start completely empty; we do not even
 * allocate space for raw arrays during initialization.
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
 * instructions array to a new array, growing it's size and finally appending
 * the new instruction to the new array.
 *
 * @param chunk A pointer to the chunk to append the new instruction to
 * @param instruction The new instruction to append to the chunk
 * @return void
 */
void write_chunk(Chunk *chunk, uint8_t byte, size_t line_number);
/*
 * @brief Free the chunk's set of arrays.
 * This function will free the memory allocated for the chunk's instructions
 * array and values array and re-initialize the chunk to an empty state by
 * calling init_chunk.
 *
 * @param chunk A pointer to the chunk to free
 * @return void
 */
void free_chunk(Chunk *chunk);
/*
 * @brief Push a new constant value to a chunk's values array.
 * This function will append a new constant to the chunk's values array. As for
 * chunk's instructions, if the chunk's values array still has free space left,
 * this function will simply push the new constant to the chunk's values array,
 * otherwise, it will allocate additional space for the new constant by copying
 * the old values array to a new chunk, growing it's values array size and
 * finally appending the new constant to the new array.
 *
 * @param chunk A pointer to the chunk to add the constant to
 * @param constant The new constant to add to the chunk
 * @return The index of the values array where the constant was added
 */
int push_constant_to_chunk(Chunk *chunk, Value constant);

#endif
