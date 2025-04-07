#include "chunk.h"
#include "memory.h"

void init_chunk(Chunk *chunk) {
  chunk->used = 0;
  chunk->capacity = 0;
  chunk->instructions = NULL;
}

void write_chunk(Chunk *chunk, uint8_t instruction) {
  if (chunk->capacity < chunk->used + 1) {
    size_t old_capacity = chunk->capacity;
    chunk->capacity = COMPUTE_INSTRUCTIONS_ARRAY_CAPACITY(old_capacity);
    chunk->instructions = GROW_INSTRUCTIONS_ARRAY(
        uint8_t, chunk->instructions, old_capacity, chunk->capacity);
  }
  chunk->instructions[chunk->used] = instruction;
  chunk->used++;
}

void free_chunk(Chunk *chunk) {
  FREE_INSTRUCTIONS_ARRAY(uint8_t, chunk->instructions, chunk->capacity);
  init_chunk(chunk);
}
