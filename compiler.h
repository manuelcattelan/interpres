#ifndef interpres_compiler_h
#define interpres_compiler_h

#include "chunk.h"
#include "parser.h"
#include "scanner.h"

/*
 * @brief Compile a set of instructions into bytecode.
 * This function takes a set of instructions in the form of a string as input
 * and compiles it into bytecode which can later be handled by our virtual
 * machine.
 *
 * @param parser A pointer to the parser that will parse the input
 * @param scanner A pointer to the scanner that will scan the input
 * @param chunk A pointer to the chunk that will hold the compiled bytecode
 * @param input The input set of instructions to compile
 * @return Whether the compilation was successful or not
 */
bool compile_input(Parser *parser, Scanner *scanner, Chunk *chunk,
                   const char *input);

#endif
