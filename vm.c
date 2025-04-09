#include "vm.h"

/* This is a global variable definition that avoids the chore of passing around
 * a pointer to the virtual machine instance to all of the functions inside this
 * module. Of course, everything bad that one can hear about global variables
 * still stands with this implementation. */
VirtualMachine vm;

void init_vm() {}
void free_vm() {}

static InterpretationResult run() {
#define READ_BYTE() (*vm.instruction_pointer++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    disassemble_instruction(
        vm.chunk, (int)(vm.instruction_pointer - vm.chunk->instructions));
#endif
    uint8_t instruction;
    switch (instruction = READ_BYTE()) {
    case OP_RETURN: {
      return INTERPRETATION_OK;
    }
    case OP_CONSTANT: {
      READ_CONSTANT();
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
