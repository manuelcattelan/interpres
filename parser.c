#include <stdbool.h>
#include <stdio.h>

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

void advance_and_validate_parser(Parser *parser, Scanner *scanner,
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
