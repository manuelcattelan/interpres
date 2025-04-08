#ifndef interpres_vm_h
#define interpres_vm_h

#include "chunk.h"

typedef struct {
  Chunk *chunk;
  uint8_t *instruction_pointer;
} VirtualMachine;

typedef enum {
  INTERPRETATION_OK,
  INTERPRETATION_COMPILE_ERROR,
  INTERPRETATION_RUNTIME_ERROR,
} InterpretationResult;

void init_vm();
void free_vm();
InterpretationResult interpret_chunk(Chunk *chunk);

#endif
