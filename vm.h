#ifndef interpres_vm_h
#define interpres_vm_h

#include "chunk.h"
#include "constant.h"
#include "scanner.h"

#define STACK_MAX_SIZE 256

/* This is our language's definition of a virtual machine. It holds a couple of
 * things: a chunk, a pointer to the chunk's next instruction to execute, the
 * stack of constants that we need for the instructions we're evaluating and a
 * pointer that points just past the last element of the stack itself. */
typedef struct {
  Chunk *chunk;
  uint8_t *instruction_pointer;
  Constant stack[STACK_MAX_SIZE];
  Constant *stack_pointer;
} VirtualMachine;

/* This enum defines the possible results of interpreting a chunk. It is used to
 * indicate whether the interpretation was successful or if there was an error
 * during compilation or execution. */
typedef enum {
  INTERPRETATION_OK,
  INTERPRETATION_COMPILE_ERROR,
  INTERPRETATION_RUNTIME_ERROR,
} InterpretationResult;

/*
 * @brief Initialize the virtual machine.
 * This function will set the stack pointer to the beginning of the stack so
 * that it can be used to store constants.
 *
 * @param vm A pointer to the virtual machine to initialize
 * @return void
 */
void init_vm(VirtualMachine *vm);
void free_vm(VirtualMachine *vm);

/*
 * @brief Scan and compile input.
 * This function will scan the input, producing tokens, and compile them into
 * bytecode that can be interpreted by our virtual machine.
 *
 * @param vm A pointer to the virtual machine
 * @param input The input to scan and compile
 * @return The result of the interpretation
 */
InterpretationResult interpret_input(VirtualMachine *vm, const char *input);
/*
 * @brief Push a constant onto the stack.
 * This function will push a constant onto the stack of the virtual machine.
 *
 * @param vm A pointer to the virtual machine to which to push the constant
 * @param constant The constant to push onto the stack
 * @return void
 */
void push_onto_stack(VirtualMachine *vm, Constant constant);
/*
 * @brief Pop a constant from the stack.
 * This function will pop a constant from the stack of the virtual machine and
 * return it to the caller.
 *
 * @param vm A pointer to the virtual machine from which to pop the constant
 * @return The constant popped from the stack
 */
Constant pop_from_stack(VirtualMachine *vm);

#endif
