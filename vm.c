#include "vm.h"
#include "chunk.h"
#include "compiler.h"
#include "constant.h"
#include "parser.h"
#include "scanner.h"
#include <stdbool.h>
#include <stdio.h>

static void reset_vm_stack(VirtualMachine *vm) {
  vm->stack_pointer = vm->stack;
}

void init_vm(VirtualMachine *vm) { reset_vm_stack(vm); }

void free_vm(VirtualMachine *vm) {}

static uint8_t read_instruction(VirtualMachine *vm) {
  return *vm->instruction_pointer++;
}

static Constant read_instruction_constant(VirtualMachine *vm) {
  return vm->chunk->constants.values[read_instruction(vm)];
}

static InterpretationResult run_vm_instruction_set(VirtualMachine *vm) {
#define BINARY_OPERATION(vm, operator)                                         \
  do {                                                                         \
    Constant second_operand = pop_from_stack(vm);                              \
    Constant first_operand = pop_from_stack(vm);                               \
    push_onto_stack(vm, first_operand operator second_operand);                \
  } while (false)

  for (;;) {
    uint8_t instruction;
    switch (instruction = read_instruction(vm)) {
    case OP_CONSTANT: {
      Constant instruction_constant = read_instruction_constant(vm);
      break;
    }
    case OP_ADD: {
      BINARY_OPERATION(vm, +);
      break;
    }
    case OP_SUBTRACT: {
      BINARY_OPERATION(vm, -);
      break;
    }
    case OP_MULTIPLY: {
      BINARY_OPERATION(vm, *);
      break;
    }
    case OP_DIVIDE: {
      BINARY_OPERATION(vm, /);
      break;
    }
    case OP_NEGATE: {
      push_onto_stack(vm, -pop_from_stack(vm));
      break;
    }
    case OP_RETURN: {
      return INTERPRETATION_OK;
    }
    }
  }
#undef BINARY_OPERATION
}

InterpretationResult interpret_input(VirtualMachine *vm, const char *input) {
  printf("inside interpret");

  Chunk chunk;
  init_chunk(&chunk);
  printf("inited chunk");
  Scanner scanner;
  init_scanner(&scanner, NULL, 0);
  printf("inited scanner");
  Parser parser;

  if (!compile_input(&parser, &scanner, &chunk, input)) {
    free_chunk(&chunk);
    return INTERPRETATION_COMPILE_ERROR;
  }

  vm->chunk = &chunk;
  vm->instruction_pointer = vm->chunk->instructions.values;
  InterpretationResult run_result = run_vm_instruction_set(vm);

  free_chunk(&chunk);
  return INTERPRETATION_OK;
}

void push_onto_stack(VirtualMachine *vm, Constant constant) {
  *vm->stack_pointer = constant;
  vm->stack_pointer++;
}

Constant pop_from_stack(VirtualMachine *vm) { return *(--vm->stack_pointer); }
