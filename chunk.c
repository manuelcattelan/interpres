#include "chunk.h"
#include "constant.h"

void init_chunk(Chunk *chunk) {
  init_instructions_array(&chunk->instructions);
  init_constants_array(&chunk->constants);
}

void free_chunk(Chunk *chunk) {
  free_instructions_array(&chunk->instructions);
  free_constants_array(&chunk->constants);
  init_chunk(chunk);
}

void push_instruction_to_chunk(Chunk *chunk, uint8_t instruction,
                               size_t line_number) {
  write_instructions_array(&chunk->instructions, instruction, line_number);
}

int push_constant_to_chunk(Chunk *chunk, Constant value) {
  write_constants_array(&chunk->constants, value);
  return chunk->constants.used - 1;
}
