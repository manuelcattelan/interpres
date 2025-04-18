#ifndef interpres_compiler_h
#define interpres_compiler_h

#include "parser.h"
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
/*
 * @brief Append a single instruction byte to the chunk.
 * This function will push an instruction byte to the chunk, along with the line
 * number of the token where the instruction was parsed from.
 *
 * @param parser A pointer to the parser that is currently compiling
 * @param currently_compiling_chunk A pointer to the chunk that is being
 * compiled
 * @param byte_to_write The byte to write to the chunk
 * @return void
 *
 */
void write_instruction_expression(Parser *parser,
                                  Chunk *currently_compiling_chunk,
                                  uint8_t byte_to_write);
/*
 * @brief Append two instruction bytes to the chunk.
 * This function will push two instruction bytes to the chunk, along with the
 * line number of the token where the instruction was parsed from. This is
 * a simple wrapper around write_instruction_expression which comes in handy
 * when we want to write constant expressions along with the constant index
 * in the chunk's constants array.
 *
 * @param parser A pointer to the parser that is currently compiling
 * @param currently_compiling_chunk A pointer to the chunk that is being
 * compiled
 * @param first_byte_to_write The first byte to write to the chunk
 * @param second_byte_to_write The second byte to write to the chunk
 * @return void
 *
 */
void write_instruction_expression_multiple(Parser *parser,
                                           Chunk *currently_compiling_chunk,
                                           uint8_t first_byte_to_write,
                                           uint8_t second_byte_to_write);
/*
 * @brief Append a constant byte to the chunk.
 * This function will push a constant byte to the chunk, meaning that it will
 * push the constant to the chunk's constants array and later push the constant
 * OpCode to the chunk together with the constant's index.
 *
 * @param parser A pointer to the parser that is currently compiling
 * @param currently_compiling_chunk A pointer to the chunk that is being
 * compiled
 * @param constant The constant to write to the chunk
 * @return void
 *
 */
void write_constant_expression(Parser *parser, Chunk *currently_compiling_chunk,
                               Constant constant);

#endif
