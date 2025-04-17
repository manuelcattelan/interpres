#ifndef interpres_compiler_h
#define interpres_compiler_h

#include "vm.h"

/*
 * @brief Compile a set of instructions into bytecode.
 * This function takes a set of instructions in the form of a string as input
 * and compiles it into bytecode which can later be handled by our virtual
 * machine.
 *
 * @param input The input set of instructions to compile
 * @param compilation_chunk A pointer to the chunk that will hold the compiled
 * bytecode
 * @return Whether the compilation was successful or not
 */
bool compile_input(const char *input, Chunk *compilation_chunk);

#endif
