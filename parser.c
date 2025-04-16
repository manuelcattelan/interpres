#include "parser.h"
#include "token.h"
#include <stdio.h>

static ParseRule *get_parsing_rule(TokenType type);

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

static void make_parser_error_at(Parser *parser, Token *error_token,
                                 const char *error_message) {
  if (parser->is_panic)
    return;
  parser->is_panic = true;
  parser->is_error = true;
}

static void make_parser_error_at_previous(Parser *parser,
                                          const char *error_message) {
  make_parser_error_at(parser, &parser->previous_token, error_message);
}

static void make_parser_error_at_current(Parser *parser,
                                         const char *error_message) {
  make_parser_error_at(parser, &parser->current_token, error_message);
}

void init_parser(Parser *parser) {
  parser->is_error = false;
  parser->is_panic = false;
}

void advance_parser(Parser *parser, Scanner *scanner) {
  parser->previous_token = parser->current_token;
  for (;;) {
    parser->current_token = scan_token(scanner);
    if (parser->current_token.type != TOKEN_ERROR)
      break;
    make_parser_error_at_current(parser, parser->current_token.lexeme_start);
  }
}

void advance_and_validate_parser(Parser *parser, Scanner *scanner,
                                 TokenType expected_token_type,
                                 const char *error_message) {
  if (parser->current_token.type == expected_token_type) {
    advance_parser(parser, scanner);
    return;
  }
  make_parser_error_at_current(parser, error_message);
}

uint8_t make_parser_constant(Parser *parser, Chunk *currently_compiling_chunk,
                             Constant constant) {
  size_t constant_index =
      push_constant_to_chunk(currently_compiling_chunk, constant);
  if (constant_index > UINT8_MAX) {
    make_parser_error_at_current(parser, "Too many constants in one chunk.");
    return 0;
  }
  return (uint8_t)constant_index;
}

static void make_numeric_expression(Parser *parser,
                                    Chunk *currently_compiling_chunk) {
  double expression_value = strtod(parser->previous_token.lexeme_start, NULL);
  write_constant_to_chunk(parser, currently_compiling_chunk, expression_value);
};

static void make_grouped_expression(Parser *parser, Scanner *scanner) {
  parse_expression(parser, scanner);
  advance_and_validate_parser(parser, scanner, TOKEN_RIGHT_PARENTHESIS,
                              "Expected ')' after expression.");
}

static void make_unary_expression(Parser *parser, Scanner *scanner,
                                  Chunk *currently_compiling_chunk) {
  TokenType operator_type = parser->previous_token.type;

  parse_expression_precedence(parser, scanner, PRECEDENCE_UNARY);

  switch (operator_type) {
  case TOKEN_MINUS:
    write_byte_to_chunk(parser, currently_compiling_chunk, OP_NEGATE);
    break;
  default:
    return;
  }
}

static void make_binary_expression(Parser *parser, Scanner *scanner,
                                   Chunk *currently_compiling_chunk) {
  TokenType operator_type = parser->previous_token.type;
  ParseRule *parsing_rule = get_parsing_rule(operator_type);
  parse_expression_precedence(parser, scanner,
                              (Precedence)(parsing_rule->precedence + 1));

  switch (operator_type) {
  case TOKEN_PLUS:
    write_byte_to_chunk(parser, currently_compiling_chunk, OP_ADD);
    break;
  case TOKEN_MINUS:
    write_byte_to_chunk(parser, currently_compiling_chunk, OP_SUBTRACT);
    break;
  case TOKEN_STAR:
    write_byte_to_chunk(parser, currently_compiling_chunk, OP_MULTIPLY);
    break;
  case TOKEN_SLASH:
    write_byte_to_chunk(parser, currently_compiling_chunk, OP_DIVIDE);
    break;
  default:
    return;
  }
}

ParseRule parsing_rules[] = {
    [TOKEN_LEFT_PARENTHESIS] = {make_grouped_expression, NULL, PRECEDENCE_NONE},
    [TOKEN_RIGHT_PARENTHESIS] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_LEFT_BRACE] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_RIGHT_BRACE] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_COMMA] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_DOT] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_MINUS] = {make_unary_expression, make_binary_expression,
                     PRECEDENCE_TERM},
    [TOKEN_PLUS] = {NULL, make_binary_expression, PRECEDENCE_TERM},
    [TOKEN_SEMICOLON] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_SLASH] = {NULL, make_binary_expression, PRECEDENCE_FACTOR},
    [TOKEN_STAR] = {NULL, make_binary_expression, PRECEDENCE_FACTOR},
    [TOKEN_BANG] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_BANG_EQUAL] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_EQUAL] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_EQUAL_EQUAL] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_GREATER] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_GREATER_EQUAL] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_LESS] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_LESS_EQUAL] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_IDENTIFIER] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_STRING] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_NUMBER] = {make_numeric_expression, NULL, PRECEDENCE_NONE},
    [TOKEN_AND] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_CLASS] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_ELSE] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_FALSE] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_FOR] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_FUNCTION] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_IF] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_NIL] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_OR] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_PRINT] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_RETURN] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_SUPER] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_THIS] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_TRUE] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_VAR] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_WHILE] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_ERROR] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_EOF] = {NULL, NULL, PRECEDENCE_NONE},
};

static ParseRule *get_parsing_rule(TokenType token_type) {
  return &parsing_rules[token_type];
}

void parse_expression_precedence(Parser *parser, Scanner *scanner,
                                 Precedence precedence) {
  advance_parser(parser, scanner);
  ParseFn parser_prefix_rule =
      get_parsing_rule(parser->previous_token.type)->prefix_function;
  if (parser_prefix_rule == NULL) {
    make_parser_error_at_current(parser, "Expect expression.");
    return;
  }

  parser_prefix_rule();
  while (precedence <=
         get_parsing_rule(parser->current_token.type)->precedence) {
    advance_parser(parser, scanner);
    ParseFn parser_infix_rule =
        get_parsing_rule(parser->previous_token.type)->infix_function;
    parser_infix_rule();
  }
}

void parse_expression(Parser *parser, Scanner *scanner) {
  parse_expression_precedence(parser, scanner, PRECEDENCE_ASSIGNMENT);
}
