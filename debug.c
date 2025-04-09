#include "debug.h"
#include "value.h"
#include <stdio.h>

static int simple_instruction(const char *instruction_name,
                              int instruction_offset) {
  printf("%s\n", instruction_name);
  return instruction_offset + 1;
}

static int constant_instruction(const char *instruction_name, Chunk *chunk,
                                int instruction_offset) {
  uint8_t instruction_constant = chunk->instructions[instruction_offset + 1];
  printf("%-16s %4d '", instruction_name, instruction_constant);
  print_value(chunk->constants.values[instruction_constant]);
  printf("'\n");
  return instruction_offset + 2;
}

int disassemble_instruction(Chunk *chunk, int instruction_offset) {
  printf("%04d ", instruction_offset);
  if (instruction_offset > 0 &&
      chunk->instruction_lines[instruction_offset] ==
          chunk->instruction_lines[instruction_offset - 1]) {
    printf("   | ");
  } else {
    printf("%4zu ", chunk->instruction_lines[instruction_offset]);
  }
  uint8_t current_instruction = chunk->instructions[instruction_offset];
  switch (current_instruction) {
  case OP_CONSTANT:
    return constant_instruction("OP_CONSTANT", chunk, instruction_offset);
  case OP_RETURN:
    return simple_instruction("OP_RETURN", instruction_offset);
  default:
    printf("Unknown opcode %d\n", current_instruction);
    return instruction_offset + 1;
  }
}

void disassemble_chunk(Chunk *chunk, const char *chunk_name) {
  printf("== %s ==\n", chunk_name);
  for (int instruction_offset = 0; instruction_offset < chunk->used;) {
    instruction_offset = disassemble_instruction(chunk, instruction_offset);
  }
}
