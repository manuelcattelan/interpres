#include <stdbool.h>

#include "compiler.h"
#include "parser.h"
#include "scanner.h"
#include "token.h"

static void write_byte(Parser *parser, Chunk *currently_compiling_chunk,
                       uint8_t byte_to_write) {
  push_instruction_to_chunk(currently_compiling_chunk, byte_to_write,
                            parser->previous_token.line_number);
}

static void write_byte_multiple(Parser *parser,
                                Chunk *currently_compiling_chunk,
                                uint8_t first_byte_to_write,
                                uint8_t second_byte_to_write) {
  write_byte(parser, currently_compiling_chunk, first_byte_to_write);
  write_byte(parser, currently_compiling_chunk, second_byte_to_write);
}

static void write_return(Parser *parser, Chunk *currently_compiling_chunk) {
  write_byte(parser, currently_compiling_chunk, OP_RETURN);
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

static void write_constant_expression(Parser *parser,
                                      Chunk *currently_compiling_chunk,
                                      Constant constant) {
  write_byte_multiple(
      parser, currently_compiling_chunk, OP_CONSTANT,
      make_constant_expression(parser, currently_compiling_chunk, constant));
}

static void end_compilation(Parser *parser, Chunk *currently_compiling_chunk) {
  write_return(parser, currently_compiling_chunk);
}

static void parse_expression_precedence(Parser *parser, Scanner *scanner,
                                        Chunk *currently_compiling_chunk,
                                        ParsePrecedence precedence);
static void parse_expression(Parser *parser, Scanner *scanner,
                             Chunk *currently_compiling_chunk);
static ParseRule *get_parsing_rule(TokenType type);

static void make_binary_expression(Parser *parser, Scanner *scanner,
                                   Chunk *currently_compiling_chunk) {
  TokenType operator_type = parser->previous_token.type;
  ParseRule *parsing_rule = get_parsing_rule(operator_type);
  parse_expression_precedence(parser, scanner, currently_compiling_chunk,
                              (ParsePrecedence)(parsing_rule->precedence + 1));
  switch (operator_type) {
  case TOKEN_PLUS:
    write_byte(parser, currently_compiling_chunk, OP_ADD);
    break;
  case TOKEN_MINUS:
    write_byte(parser, currently_compiling_chunk, OP_SUBTRACT);
    break;
  case TOKEN_STAR:
    write_byte(parser, currently_compiling_chunk, OP_MULTIPLY);
    break;
  case TOKEN_SLASH:
    write_byte(parser, currently_compiling_chunk, OP_DIVIDE);
    break;
  default:
    return;
  }
}

static void make_grouping_expression(Parser *parser, Scanner *scanner,
                                     Chunk *currently_compiling_chunk) {
  parse_expression(parser, scanner, currently_compiling_chunk);
  advance_and_validate_parser(parser, scanner, TOKEN_RIGHT_PARENTHESIS,
                              "expected ')' after expression.");
}

static void make_numeric_expresion(Parser *parser, Scanner *scanner,
                                   Chunk *currently_compiling_chunk) {
  double numeric_value = strtod(parser->previous_token.lexeme_start, NULL);
  write_constant_expression(parser, currently_compiling_chunk, numeric_value);
}

static void make_unary_expression(Parser *parser, Scanner *scanner,
                                  Chunk *currently_compiling_chunk) {
  TokenType operator_type = parser->previous_token.type;
  parse_expression_precedence(parser, scanner, currently_compiling_chunk,
                              PRECEDENCE_UNARY);
  switch (operator_type) {
  case TOKEN_MINUS:
    write_byte(parser, currently_compiling_chunk, OP_NEGATE);
    break;
  default:
    return;
  }
}

ParseRule parsing_rules[] = {
    [TOKEN_LEFT_PARENTHESIS] = {make_grouping_expression, NULL,
                                PRECEDENCE_NONE},
    [TOKEN_RIGHT_PARENTHESIS] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_LEFT_BRACE] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_RIGHT_BRACE] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_DOT] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_COMMA] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_SEMICOLON] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_PLUS] = {NULL, make_binary_expression, PRECEDENCE_TERM},
    [TOKEN_MINUS] = {make_unary_expression, make_binary_expression,
                     PRECEDENCE_TERM},
    [TOKEN_STAR] = {NULL, make_binary_expression, PRECEDENCE_FACTOR},
    [TOKEN_SLASH] = {NULL, make_binary_expression, PRECEDENCE_FACTOR},
    [TOKEN_BANG] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_BANG_EQUAL] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_EQUAL] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_EQUAL_EQUAL] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_GREATER] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_GREATER_EQUAL] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_LESS] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_LESS_EQUAL] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_IDENTIFIER] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_VAR] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_STRING] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_NUMBER] = {make_numeric_expresion, NULL, PRECEDENCE_NONE},
    [TOKEN_NIL] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_IF] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_ELSE] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_AND] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_OR] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_TRUE] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_FALSE] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_FUNCTION] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_FOR] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_WHILE] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_RETURN] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_CLASS] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_THIS] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_SUPER] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_PRINT] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_ERROR] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_EOF] = {NULL, NULL, PRECEDENCE_NONE},
};

static void parse_expression_precedence(Parser *parser, Scanner *scanner,
                                        Chunk *currently_compiling_chunk,
                                        ParsePrecedence precedence) {
  advance_parser(parser, scanner);
  ParseFunction prefix_parsing_rule =
      get_parsing_rule(parser->previous_token.type)->prefix_function;
  if (prefix_parsing_rule == NULL) {
    parser_error_at_previous(parser, "expected expression.");
    return;
  }
  prefix_parsing_rule(parser, scanner, currently_compiling_chunk);
  while (precedence <=
         get_parsing_rule(parser->current_token.type)->precedence) {
    advance_parser(parser, scanner);
    ParseFunction infix_parsing_rule =
        get_parsing_rule(parser->previous_token.type)->infix_function;
    infix_parsing_rule(parser, scanner, currently_compiling_chunk);
  }
}

static void parse_expression(Parser *parser, Scanner *scanner,
                             Chunk *currently_compiling_chunk) {
  parse_expression_precedence(parser, scanner, currently_compiling_chunk,
                              PRECEDENCE_ASSIGNMENT);
}

static ParseRule *get_parsing_rule(TokenType type) {
  return &parsing_rules[type];
}

bool compile_input(const char *input, Chunk *compilation_chunk) {
  Parser parser;
  init_parser(&parser);
  Scanner scanner;
  init_scanner(&scanner, input);
  Chunk *currently_compiling_chunk = compilation_chunk;
  advance_parser(&parser, &scanner);
  parse_expression(&parser, &scanner, currently_compiling_chunk);
  advance_and_validate_parser(&parser, &scanner, TOKEN_EOF,
                              "expected end of expression.");
  end_compilation(&parser, currently_compiling_chunk);
  return !parser.is_error;
}
