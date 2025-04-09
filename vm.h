#ifndef interpres_vm_h
#define interpres_vm_h

#include "chunk.h"
#include "value.h"

#define STACK_MAX_SIZE 256

/* This is our language's definition of a virtual machine. It is a simple object
 * that holds both a chunk and a pointer to the chunk's instruction that we are
 * currently executing. It also holds the stack of values that we need for the
 * instructions we're constructing and a pointer that points just past the last
 * element of the stack itself. */
typedef struct {
  Chunk *chunk;
  uint8_t *instruction_pointer;
  Value stack[STACK_MAX_SIZE];
  Value *stack_top;
} VirtualMachine;

/* This enum defines the possible results of interpreting a chunk. It is used to
 * indicate whether the interpretation was successful or if there was an error
 * during compilation or execution. */
typedef enum {
  INTERPRETATION_OK,
  INTERPRETATION_COMPILE_ERROR,
  INTERPRETATION_RUNTIME_ERROR,
} InterpretationResult;

void init_vm();
void free_vm();

/*
 * @brief Interpret a chunk of instructions.
 * This function will execute the instructions in the chunk and return the
 * result of the interpretation.
 *
 * @param chunk A pointer to the chunk to interpret
 * @return InterpretationResult The result of the interpretation
 */
InterpretationResult interpret_chunk(Chunk *chunk);
/*
 * @brief Push a value onto the stack.
 * This function will push a value onto the stack of the virtual machine.
 *
 * @param value The value to push onto the stack
 * @return void
 */
void push(Value value);
/*
 * @brief Pop a value from the stack.
 * This function will pop a value from the stack of the virtual machine.
 *
 * @return Value The value popped from the stack
 */
Value pop();

#endif
