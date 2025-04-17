#include <stdbool.h>
#include <stdio.h>

#include "compiler.h"

static void init_stack(VirtualMachine *vm) { vm->stack_pointer = vm->stack; }

static InterpretationResult run_input_compiled(VirtualMachine *vm) {
#define READ_INSTRUCTION(vm) (*vm->instruction_pointer++)
#define READ_INSTRUCTION_CONSTANT(vm)                                          \
  (vm->chunk->constants.values[READ_INSTRUCTION(vm)])
#define BINARY_OPERATION(vm, operator)                                         \
  do {                                                                         \
    Constant second_operand = pop_from_stack(vm);                              \
    Constant first_operand = pop_from_stack(vm);                               \
    push_onto_stack(vm, first_operand operator second_operand);                \
  } while (false)

  for (;;) {
    uint8_t instruction;
    switch (instruction = READ_INSTRUCTION(vm)) {
    case OP_CONSTANT: {
      Constant instruction_constant = READ_INSTRUCTION_CONSTANT(vm);
      push_onto_stack(vm, instruction_constant);
      break;
    }
    case OP_ADD:
      BINARY_OPERATION(vm, +);
      break;
    case OP_SUBTRACT:
      BINARY_OPERATION(vm, -);
      break;
    case OP_MULTIPLY:
      BINARY_OPERATION(vm, *);
      break;
    case OP_DIVIDE:
      BINARY_OPERATION(vm, /);
      break;
    case OP_NEGATE:
      push_onto_stack(vm, -pop_from_stack(vm));
      break;
    case OP_RETURN: {
      printf("%g\n", pop_from_stack(vm));
      return INTERPRETATION_OK;
    }
    }
  }

#undef READ_INSTRUCTION
#undef READ_INSTRUCTION_CONSTANT
#undef BINARY_OPERATION
}

void init_vm(VirtualMachine *vm) { init_stack(vm); }

void free_vm(VirtualMachine *vm) {}

InterpretationResult interpret_input(VirtualMachine *vm, const char *input) {
  Chunk chunk;
  init_chunk(&chunk);

  if (!compile_input(input, &chunk)) {
    free_chunk(&chunk);
    return INTERPRETATION_COMPILE_ERROR;
  }

  vm->chunk = &chunk;
  vm->instruction_pointer = vm->chunk->instructions.values;
  InterpretationResult interpretation_result = run_input_compiled(vm);

  free_chunk(&chunk);
  return interpretation_result;
}

void push_onto_stack(VirtualMachine *vm, Constant constant) {
  *vm->stack_pointer = constant;
  vm->stack_pointer++;
}

Constant pop_from_stack(VirtualMachine *vm) { return *(--vm->stack_pointer); }
