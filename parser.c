#include <stdbool.h>
#include <stdio.h>

#include "compiler.h"
#include "parser.h"

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
    parser_error_at_current(parser, parser->current_token.lexeme_start);
  }
}

void advance_parser_and_validate_token(Parser *parser, Scanner *scanner,
                                       TokenType type, const char *message) {
  if (parser->current_token.type == type) {
    advance_parser(parser, scanner);
    return;
  }
  parser_error_at_current(parser, message);
}

void parser_error_at(Parser *parser, Token *token, const char *error_message) {
  if (parser->is_panic)
    return;
  parser->is_panic = true;
  parser->is_error = true;
  fprintf(stderr, "[line:%d] error", (int)token->line_number);
  if (token->type == TOKEN_EOF) {
    fprintf(stderr, " at EOF");
  } else if (token->type == TOKEN_ERROR) {
  } else {
    fprintf(stderr, " at '%.*s'", (int)token->lexeme_length,
            token->lexeme_start);
  }
  fprintf(stderr, ": %s\n", error_message);
}

void parser_error_at_previous(Parser *parser, const char *error_message) {
  parser_error_at(parser, &parser->previous_token, error_message);
}

void parser_error_at_current(Parser *parser, const char *error_message) {
  parser_error_at(parser, &parser->current_token, error_message);
}

void parse_expression_precedence(Parser *parser, Scanner *scanner,
                                 Chunk *currently_compiling_chunk,
                                 ParsingPrecedence parsing_precedence) {
  advance_parser(parser, scanner);
  ParsingFunction prefix_parsing_rule =
      get_parsing_rule(parser->previous_token.type)->prefix_function;
  if (prefix_parsing_rule == NULL) {
    parser_error_at_previous(parser, "expected expression.");
    return;
  }
  prefix_parsing_rule(parser, scanner, currently_compiling_chunk);
  while (parsing_precedence <=
         get_parsing_rule(parser->current_token.type)->parsing_precedence) {
    advance_parser(parser, scanner);
    ParsingFunction infix_parsing_rule =
        get_parsing_rule(parser->previous_token.type)->infix_function;
    infix_parsing_rule(parser, scanner, currently_compiling_chunk);
  }
}

void parse_expression(Parser *parser, Scanner *scanner,
                      Chunk *currently_compiling_chunk) {
  parse_expression_precedence(parser, scanner, currently_compiling_chunk,
                              PRECEDENCE_ASSIGNMENT);
}

static void parse_binary_expression(Parser *parser, Scanner *scanner,
                                    Chunk *currently_compiling_chunk) {
  TokenType operator_type = parser->previous_token.type;
  ParsingRule *parsing_rule = get_parsing_rule(operator_type);
  parse_expression_precedence(
      parser, scanner, currently_compiling_chunk,
      (ParsingPrecedence)(parsing_rule->parsing_precedence + 1));
  switch (operator_type) {
  case TOKEN_PLUS:
    write_instruction_expression(parser, currently_compiling_chunk, OP_ADD);
    break;
  case TOKEN_MINUS:
    write_instruction_expression(parser, currently_compiling_chunk,
                                 OP_SUBTRACT);
    break;
  case TOKEN_STAR:
    write_instruction_expression(parser, currently_compiling_chunk,
                                 OP_MULTIPLY);
    break;
  case TOKEN_SLASH:
    write_instruction_expression(parser, currently_compiling_chunk, OP_DIVIDE);
    break;
  default:
    return;
  }
}

static void parse_grouping_expression(Parser *parser, Scanner *scanner,
                                      Chunk *currently_compiling_chunk) {
  parse_expression(parser, scanner, currently_compiling_chunk);
  advance_parser_and_validate_token(parser, scanner, TOKEN_RIGHT_PARENTHESIS,
                                    "expected ')' after expression.");
}

static void parse_numeric_expresion(Parser *parser, Scanner *scanner,
                                    Chunk *currently_compiling_chunk) {
  double numeric_value = strtod(parser->previous_token.lexeme_start, NULL);
  write_constant_expression(parser, currently_compiling_chunk, numeric_value);
}

static void parse_unary_expression(Parser *parser, Scanner *scanner,
                                   Chunk *currently_compiling_chunk) {
  TokenType operator_type = parser->previous_token.type;
  parse_expression_precedence(parser, scanner, currently_compiling_chunk,
                              PRECEDENCE_UNARY);
  switch (operator_type) {
  case TOKEN_MINUS:
    write_instruction_expression(parser, currently_compiling_chunk, OP_NEGATE);
    break;
  default:
    return;
  }
}

ParsingRule token_type_to_parsing_rules[] = {
    [TOKEN_LEFT_PARENTHESIS] = {parse_grouping_expression, NULL,
                                PRECEDENCE_NONE},
    [TOKEN_RIGHT_PARENTHESIS] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_LEFT_BRACE] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_RIGHT_BRACE] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_DOT] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_COMMA] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_SEMICOLON] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_PLUS] = {NULL, parse_binary_expression, PRECEDENCE_TERM},
    [TOKEN_MINUS] = {parse_unary_expression, parse_binary_expression,
                     PRECEDENCE_TERM},
    [TOKEN_STAR] = {NULL, parse_binary_expression, PRECEDENCE_FACTOR},
    [TOKEN_SLASH] = {NULL, parse_binary_expression, PRECEDENCE_FACTOR},
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
    [TOKEN_NUMBER] = {parse_numeric_expresion, NULL, PRECEDENCE_NONE},
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

ParsingRule *get_parsing_rule(TokenType token_type) {
  return &token_type_to_parsing_rules[token_type];
}
