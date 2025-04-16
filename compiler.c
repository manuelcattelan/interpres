#include "compiler.h"
#include "chunk.h"
#include "parser.h"
#include "scanner.h"
#include "token.h"
#include <stdio.h>

static void write_byte_to_chunk(Parser *parser,
                                Chunk *currently_compiling_chunk,
                                uint8_t instruction) {
  push_instruction_to_chunk(currently_compiling_chunk, instruction,
                            parser->previous_token.line_number);
}
static void write_byte_multiple_to_chunk(Parser *parser,
                                         Chunk *currently_compiling_chunk,
                                         uint8_t first_instruction,
                                         uint8_t second_instruction) {
  write_byte_to_chunk(parser, currently_compiling_chunk, first_instruction);
  write_byte_to_chunk(parser, currently_compiling_chunk, second_instruction);
}

static void write_byte_return_to_chunk(Parser *parser,
                                       Chunk *currently_compiling_chunk) {
  write_byte_to_chunk(parser, currently_compiling_chunk, OP_RETURN);
}

static void write_constant_to_chunk(Parser *parser,
                                    Chunk *currently_compiling_chunk,
                                    Constant constant) {
  write_byte_multiple_to_chunk(
      parser, currently_compiling_chunk, OP_CONSTANT,
      make_parser_constant(parser, currently_compiling_chunk, constant));
}

static void end_compilation(Parser *parser, Chunk *currently_compiling_chunk) {
  write_byte_return_to_chunk(parser, currently_compiling_chunk);
}

bool compile_input(Parser *parser, Scanner *scanner, Chunk *chunk,
                   const char *input) {
  init_scanner(scanner, input, 1);
  init_parser(parser);
  Chunk *currently_compiling_chunk = chunk;

  advance_parser(parser, scanner);
  parse_expression(parser, scanner);
  advance_and_validate_parser(parser, scanner, TOKEN_EOF, "Expected EOF.");
  end_compilation(parser, currently_compiling_chunk);

  return !parser->is_error;
}
