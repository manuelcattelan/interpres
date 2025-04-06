#include "chunk.h"

void init_chunk(Chunk *chunk) {
  chunk->used = 0;
  chunk->capacity = 0;
  chunk->instructions = NULL;
}
