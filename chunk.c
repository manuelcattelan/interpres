#include "chunk.h"
#include "memory.h"
#include "value.h"

void init_chunk(Chunk *chunk) {
  chunk->used = 0;
  chunk->capacity = 0;
  chunk->instructions = NULL;
  chunk->instruction_lines = NULL;
  init_value_array(&chunk->constants);
}

void write_chunk(Chunk *chunk, uint8_t instruction, size_t line_number) {
  if (chunk->capacity < chunk->used + 1) {
    size_t old_capacity = chunk->capacity;
    chunk->capacity = COMPUTE_ARRAY_CAPACITY(old_capacity);
    chunk->instructions =
        GROW_ARRAY(uint8_t, chunk->instructions, old_capacity, chunk->capacity);
    chunk->instruction_lines = GROW_ARRAY(size_t, chunk->instruction_lines,
                                          old_capacity, chunk->capacity);
  }
  chunk->instructions[chunk->used] = instruction;
  chunk->instruction_lines[chunk->used] = line_number;
  chunk->used++;
}

void free_chunk(Chunk *chunk) {
  FREE_ARRAY(uint8_t, chunk->instructions, chunk->capacity);
  FREE_ARRAY(size_t, chunk->instruction_lines, chunk->capacity);
  free_value_array(&chunk->constants);
  init_chunk(chunk);
}

int push_constant_to_chunk(Chunk *chunk, Value value) {
  write_value_array(&chunk->constants, value);
  return chunk->constants.used - 1;
}
