#ifndef interpres_chunk_h
#define interpres_chunk_h

#include "constant.h"
#include "instruction.h"

/* Each instruction in bytecode format has a one-byte operation code that
 * represents what kind of operation we're dealing with from arithmetic
 * operations to looking up variables, returning from somewhere, etc... */
typedef enum {
  OP_RETURN,
  OP_CONSTANT,
  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_NEGATE,
} OpCode;
/* A chunk is nothing more than sequences of bytecode instructions and the
 * constants that make up those instructions; notice that both are implemented
 * as dynamic arrays since they need to grow and shrink in size at runtime. */
typedef struct {
  InstructionsArray instructions;
  ConstantsArray constants;
} Chunk;
/*
 * @brief Initialize a new chunk.
 * We set a starting value of 0 for "used" and "capacity", while the chunk's
 * instructions array and constants array start completely empty; we do not even
 * allocate space for raw arrays during initialization.
 *
 * @param chunk A pointer to the chunk to initialize
 * @return void
 */
void init_chunk(Chunk *chunk);
/*
 * @brief Free the chunk's set of arrays.
 * This function will free the memory allocated for the chunk's instructions
 * array and constants array and re-initialize the chunk to an empty state by
 * calling init_chunk.
 *
 * @param chunk A pointer to the chunk to free
 * @return void
 */
void free_chunk(Chunk *chunk);
/*
 * @brief Append a new instruction to the chunk's instructions array.
 * If the chunk's instructions array still has free space left, this function
 * will simply push the new instruction to the it. Otherwise, it will allocate
 * additional space for the new instruction by copying the old instructions
 * array to a new one, growing it's size and finally appending the new
 * instruction to the just created array.
 *
 * @param chunk A pointer to the chunk to append the new instruction to
 * @param instruction The new instruction to add to the chunk
 * @param line_number The line number of the instruction to append to the array
 * @return void
 */
void push_instruction_to_chunk(Chunk *chunk, uint8_t instruction,
                               size_t line_number);
/*
 * @brief Append a new constant to a chunk's constants array.
 * If the chunk's constants array still has free space left, this function will
 * simply push the new constant to the it. Otherwise, it will allocate
 * additional space for the new constant by copying the old constants array to a
 * new one, growing it's size and finally appending the new constant to the just
 * created array.
 *
 * @param chunk A pointer to the chunk to append the new constant to
 * @param constant The new constant to add to the chunk
 * @return The index of the constants array where the constant was added
 */
size_t push_constant_to_chunk(Chunk *chunk, Constant constant);

#endif
