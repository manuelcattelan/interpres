#include "vm.h"
#include "compiler.h"
#include "debug.h"
#include <stdio.h>

/* This is a global variable definition that avoids the chore of passing around
 * a pointer to the virtual machine instance to all of the functions inside this
 * module. Of course, everything bad that one can hear about global variables
 * still stands with this implementation. */
VirtualMachine vm;

/*
 * @brief Initialize the stack of the virtual machine.
 * This function will initialize the stack of the virtual machine to its initial
 * state by making the pointer of the next element to push onto the stack point
 * to the beginning of the stack.
 *
 * @return void
 */
static void init_stack() { vm.stack_top = vm.stack; }

void init_vm() { init_stack(); }
void free_vm() {}

void push_onto_stack(Value value) {
  *vm.stack_top = value;
  vm.stack_top++;
}

Value pop_from_stack() {
  vm.stack_top--;
  return *vm.stack_top;
}

static InterpretationResult run() {
#define READ_BYTE() (*vm.instruction_pointer++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op)                                                          \
  do {                                                                         \
    double second_operand = pop_from_stack();                                  \
    double first_operand = pop_from_stack();                                   \
    push_onto_stack(first_operand op second_operand);                          \
  } while (false)
  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    printf("          ");
    for (Value *stack_slot = vm.stack; stack_slot < vm.stack_top;
         stack_slot++) {
      printf("[ ");
      print_value(*stack_slot);
      printf(" ]");
    }
    printf("\n");
    disassemble_instruction(
        vm.chunk, (int)(vm.instruction_pointer - vm.chunk->instructions));
#endif
    uint8_t instruction;
    switch (instruction = READ_BYTE()) {
    case OP_CONSTANT: {
      Value constant = READ_CONSTANT();
      push_onto_stack(constant);
      break;
    }
    case OP_NEGATE:
      push_onto_stack(-pop_from_stack());
      break;
    case OP_ADD:
      BINARY_OP(+);
      break;
    case OP_SUBTRACT:
      BINARY_OP(-);
      break;
    case OP_MULTIPLY:
      BINARY_OP(*);
      break;
    case OP_DIVIDE:
      BINARY_OP(/);
      break;
    case OP_RETURN: {
      print_value(pop_from_stack());
      printf("\n");
      return INTERPRETATION_OK;
    }
    }
  }
#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

InterpretationResult interpret(const char *input) {
  compile(input);
  return INTERPRETATION_OK;
}
