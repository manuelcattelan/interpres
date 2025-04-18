#include <string.h>

#include "token.h"

static bool token_is_alphabetic(char lexeme_character) {
  return (lexeme_character >= 'a' && lexeme_character <= 'z') ||
         (lexeme_character >= 'A' && lexeme_character <= 'Z') ||
         lexeme_character == '_';
}

static bool token_is_digit(char lexeme_character) {
  return lexeme_character >= '0' && lexeme_character <= '9';
}

static Token make_token(Scanner *scanner, TokenType token_type) {
  Token token;
  token.type = token_type;
  token.lexeme_start = scanner->lexeme_start;
  token.lexeme_length =
      (size_t)(scanner->lexeme_current - scanner->lexeme_start);
  token.line_number = scanner->line_number;
  return token;
}

static Token make_error_token(Scanner *scanner, const char *error_message) {
  Token token;
  token.type = TOKEN_ERROR;
  token.lexeme_start = error_message;
  token.lexeme_length = (size_t)strlen(error_message);
  token.line_number = scanner->line_number;
  return token;
}

static void skip_whitespaces(Scanner *scanner) {
  for (;;) {
    char lexeme_character;
    switch (lexeme_character = peek_scanner(scanner)) {
    case ' ':
    case '\r':
    case '\t':
      advance_scanner(scanner);
      break;
    default:
      return;
    }
  }
}

static void skip_newlines(Scanner *scanner) {
  for (;;) {
    char lexeme_character = peek_scanner(scanner);
    if (lexeme_character == '\n') {
      scanner->line_number++;
      advance_scanner(scanner);
      break;
    }
    return;
  }
}

static void skip_comments(Scanner *scanner) {
  for (;;) {
    char lexeme_character = peek_scanner(scanner);
    if (lexeme_character == '/') {
      if (peek_scanner_next(scanner) == '/') {
        while (peek_scanner(scanner) != '\n' && !scanner_is_eof(scanner))
          advance_scanner(scanner);
      } else {
        return;
      }
    }
    return;
  }
}

static TokenType match_identifier_token_type(Scanner *scanner,
                                             size_t remaining_start,
                                             size_t remaining_length,
                                             const char *remaining,
                                             TokenType token_type) {
  if (scanner->lexeme_current - scanner->lexeme_start ==
          remaining_start + remaining_length &&
      memcmp(scanner->lexeme_start + remaining_start, remaining,
             remaining_length) == 0) {
    return token_type;
  }
  return TOKEN_IDENTIFIER;
}

static TokenType make_identifier_token_type(Scanner *scanner) {
  switch (scanner->lexeme_start[0]) {
  case 'a':
    return match_identifier_token_type(scanner, 1, 2, "nd", TOKEN_AND);
  case 'c':
    return match_identifier_token_type(scanner, 1, 4, "lass", TOKEN_CLASS);
  case 'e':
    return match_identifier_token_type(scanner, 1, 3, "lse", TOKEN_ELSE);
  case 'f':
    if (scanner->lexeme_current - scanner->lexeme_start > 1) {
      switch (scanner->lexeme_start[1]) {
      case 'a':
        return match_identifier_token_type(scanner, 2, 3, "lse", TOKEN_FALSE);
      case 'o':
        return match_identifier_token_type(scanner, 2, 1, "r", TOKEN_FOR);
      case 'u':
        return match_identifier_token_type(scanner, 2, 1, "n", TOKEN_FUNCTION);
      }
    }
    break;
  case 'i':
    return match_identifier_token_type(scanner, 1, 1, "f", TOKEN_IF);
  case 'n':
    return match_identifier_token_type(scanner, 1, 2, "il", TOKEN_NIL);
  case 'o':
    return match_identifier_token_type(scanner, 1, 1, "r", TOKEN_OR);
  case 'p':
    return match_identifier_token_type(scanner, 1, 4, "rint", TOKEN_PRINT);
  case 'r':
    return match_identifier_token_type(scanner, 1, 5, "eturn", TOKEN_RETURN);
  case 's':
    return match_identifier_token_type(scanner, 1, 4, "uper", TOKEN_SUPER);
  case 't':
    if (scanner->lexeme_current - scanner->lexeme_start > 1) {
      switch (scanner->lexeme_start[1]) {
      case 'h':
        return match_identifier_token_type(scanner, 2, 2, "is", TOKEN_THIS);
      case 'r':
        return match_identifier_token_type(scanner, 2, 2, "ue", TOKEN_TRUE);
      }
    }
    break;
  case 'v':
    return match_identifier_token_type(scanner, 1, 2, "ar", TOKEN_VAR);
  case 'w':
    return match_identifier_token_type(scanner, 1, 4, "hile", TOKEN_WHILE);
  }
  return TOKEN_IDENTIFIER;
}

static Token make_identifier_token(Scanner *scanner) {
  while (token_is_alphabetic(peek_scanner(scanner)) ||
         token_is_digit(peek_scanner(scanner)))
    advance_scanner(scanner);
  return make_token(scanner, make_identifier_token_type(scanner));
}

static Token make_numeric_token(Scanner *scanner) {
  while (token_is_digit(peek_scanner(scanner)))
    advance_scanner(scanner);
  if (peek_scanner(scanner) == '.' &&
      token_is_digit(peek_scanner_next(scanner))) {
    advance_scanner(scanner);
    while (token_is_digit(peek_scanner(scanner)))
      advance_scanner(scanner);
  }
  return make_token(scanner, TOKEN_NUMBER);
}

static Token make_string_token(Scanner *scanner) {
  while (peek_scanner(scanner) != '"' && !scanner_is_eof(scanner)) {
    if (peek_scanner(scanner) == '\n')
      scanner->line_number++;
    advance_scanner(scanner);
  }
  if (scanner_is_eof(scanner))
    return make_error_token(scanner, "Unterminated string.");
  advance_scanner(scanner);
  return make_token(scanner, TOKEN_STRING);
}

Token scan_token(Scanner *scanner) {
  skip_whitespaces(scanner);
  skip_newlines(scanner);
  skip_comments(scanner);
  scanner->lexeme_start = scanner->lexeme_current;
  if (scanner_is_eof(scanner))
    return make_token(scanner, TOKEN_EOF);
  char lexeme_character = advance_scanner(scanner);
  if (token_is_digit(lexeme_character))
    return make_numeric_token(scanner);
  if (token_is_alphabetic(lexeme_character))
    return make_identifier_token(scanner);
  switch (lexeme_character) {
  case '(':
    return make_token(scanner, TOKEN_LEFT_PARENTHESIS);
  case ')':
    return make_token(scanner, TOKEN_RIGHT_PARENTHESIS);
  case '{':
    return make_token(scanner, TOKEN_LEFT_BRACE);
  case '}':
    return make_token(scanner, TOKEN_RIGHT_BRACE);
  case ';':
    return make_token(scanner, TOKEN_SEMICOLON);
  case ',':
    return make_token(scanner, TOKEN_COMMA);
  case '.':
    return make_token(scanner, TOKEN_DOT);
  case '-':
    return make_token(scanner, TOKEN_MINUS);
  case '+':
    return make_token(scanner, TOKEN_PLUS);
  case '/':
    return make_token(scanner, TOKEN_SLASH);
  case '*':
    return make_token(scanner, TOKEN_STAR);
  case '!':
    return make_token(scanner, match_current_scanner_token(scanner, '=')
                                   ? TOKEN_BANG_EQUAL
                                   : TOKEN_BANG);
  case '=':
    return make_token(scanner, match_current_scanner_token(scanner, '=')
                                   ? TOKEN_EQUAL_EQUAL
                                   : TOKEN_EQUAL);
  case '<':
    return make_token(scanner, match_current_scanner_token(scanner, '=')
                                   ? TOKEN_LESS_EQUAL
                                   : TOKEN_LESS);
  case '>':
    return make_token(scanner, match_current_scanner_token(scanner, '=')
                                   ? TOKEN_GREATER_EQUAL
                                   : TOKEN_GREATER);
  case '"':
    return make_string_token(scanner);
  }
  return make_error_token(scanner, "Unexpected character.");
}
