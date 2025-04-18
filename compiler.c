#include <stdbool.h>

#include "parser.h"

void write_instruction_expression(Parser *parser,
                                  Chunk *currently_compiling_chunk,
                                  uint8_t byte_to_write) {
  push_instruction_to_chunk(currently_compiling_chunk, byte_to_write,
                            parser->previous_token.line_number);
}

void write_instruction_expression_multiple(Parser *parser,
                                           Chunk *currently_compiling_chunk,
                                           uint8_t first_byte_to_write,
                                           uint8_t second_byte_to_write) {
  write_instruction_expression(parser, currently_compiling_chunk,
                               first_byte_to_write);
  write_instruction_expression(parser, currently_compiling_chunk,
                               second_byte_to_write);
}

static uint8_t make_constant_expression(Parser *parser,
                                        Chunk *currently_compiling_chunk,
                                        Constant constant) {
  size_t constant_index =
      push_constant_to_chunk(currently_compiling_chunk, constant);
  if (constant_index > UINT8_MAX) {
    parser_error_at_previous(parser, "too many constants in one chunk.");
    return 0;
  }
  return (uint8_t)constant_index;
}

void write_constant_expression(Parser *parser, Chunk *currently_compiling_chunk,
                               Constant constant) {
  write_instruction_expression_multiple(
      parser, currently_compiling_chunk, OP_CONSTANT,
      make_constant_expression(parser, currently_compiling_chunk, constant));
}

static void write_return(Parser *parser, Chunk *currently_compiling_chunk) {
  write_instruction_expression(parser, currently_compiling_chunk, OP_RETURN);
}

static void end_compilation(Parser *parser, Chunk *currently_compiling_chunk) {
  write_return(parser, currently_compiling_chunk);
}

bool compile_input(const char *input, Chunk *compilation_chunk) {
  Parser parser;
  init_parser(&parser);
  Scanner scanner;
  init_scanner(&scanner, input);
  Chunk *currently_compiling_chunk = compilation_chunk;
  advance_parser(&parser, &scanner);
  parse_expression(&parser, &scanner, currently_compiling_chunk);
  advance_parser_and_validate_token(&parser, &scanner, TOKEN_EOF,
                                    "expected end of expression.");
  end_compilation(&parser, currently_compiling_chunk);
  return !parser.is_error;
}
