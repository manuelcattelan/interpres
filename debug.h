#ifndef interpres_debug_h
#define interpres_debug_h

#include "chunk.h"

/*
 * @brief Disassemble a chunk's instructions array.
 * This function will print the disassembled instructions of a chunk to the
 * standard output together with the chunk's name.
 *
 * @param chunk A pointer to the chunk to disassemble
 * @param chunk_name The name of the chunk
 * @return void
 */
void disassemble_chunk(Chunk *chunk, const char *chunk_name);
/*
 * @brief Disassemble a single chunk instruction.
 * This function will print the disassembled instruction of a chunk to the
 * standard output.
 *
 * @param chunk A pointer to the chunk to disassemble
 * @param instruction_offset The offset of the instruction to disassemble
 * @return int The offset of the next instruction to disassemble
 */
int disassemble_instruction(Chunk *chunk, int instruction_offset);

#endif
