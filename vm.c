#include "vm.h"
#include "debug.h"
#include <stdio.h>

/* This is a global variable definition that avoids the chore of passing around
 * a pointer to the virtual machine instance to all of the functions inside this
 * module. Of course, everything bad that one can hear about global variables
 * still stands with this implementation. */
VirtualMachine vm;

static void reset_stack() { vm.stack_top = vm.stack; }

void init_vm() { reset_stack(); }
void free_vm() {}

void push(Value value) {
  *vm.stack_top = value;
  vm.stack_top++;
}

Value pop() {
  vm.stack_top--;
  return *vm.stack_top;
}

static InterpretationResult run() {
#define READ_BYTE() (*vm.instruction_pointer++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
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
    case OP_RETURN: {
      print_value(pop());
      printf("\n");
      return INTERPRETATION_OK;
    }
    case OP_CONSTANT: {
      Value constant = READ_CONSTANT();
      push(constant);
      break;
    }
    }
  }
#undef READ_BYTE
#undef READ_CONSTANT
}

InterpretationResult interpret_chunk(Chunk *chunk) {
  vm.chunk = chunk;
  vm.instruction_pointer = vm.chunk->instructions;
  return run();
}
