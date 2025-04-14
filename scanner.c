#include "scanner.h"
#include <string.h>

/* This is a global variable definition that avoids the chore of passing around
 * a pointer to the scanner instance to all of the functions inside this module.
 * Of course, everything bad that one can hear about global variables still
 * stands with this implementation. */
Scanner scanner;

static TokenType check_identifier_keyword(int keyword_start, int keyword_length,
                                          const char *keyword_remaining,
                                          TokenType token_type) {
  if (scanner.lexeme_current - scanner.lexeme_start ==
          keyword_start + keyword_length &&
      memcmp(scanner.lexeme_start + keyword_start, keyword_remaining,
             keyword_length) == 0) {
    return token_type;
  }

  return TOKEN_IDENTIFIER;
}

static TokenType get_identifier_type() {
  switch (scanner.lexeme_start[0]) {
  case 'a':
    return check_identifier_keyword(1, 2, "nd", TOKEN_AND);
  case 'c':
    return check_identifier_keyword(1, 4, "lass", TOKEN_CLASS);
  case 'e':
    return check_identifier_keyword(1, 3, "lse", TOKEN_ELSE);
  case 'f':
    if (scanner.lexeme_current - scanner.lexeme_start > 1) {
      switch (scanner.lexeme_start[1]) {
      case 'a':
        return check_identifier_keyword(2, 3, "lse", TOKEN_FALSE);
      case 'o':
        return check_identifier_keyword(2, 1, "r", TOKEN_FOR);
      case 'u':
        return check_identifier_keyword(2, 1, "n", TOKEN_FUN);
      }
    }
    break;
  case 'i':
    return check_identifier_keyword(1, 1, "f", TOKEN_IF);
  case 'n':
    return check_identifier_keyword(1, 2, "il", TOKEN_NIL);
  case 'o':
    return check_identifier_keyword(1, 1, "r", TOKEN_OR);
  case 'p':
    return check_identifier_keyword(1, 4, "rint", TOKEN_PRINT);
  case 'r':
    return check_identifier_keyword(1, 5, "eturn", TOKEN_RETURN);
  case 's':
    return check_identifier_keyword(1, 4, "uper", TOKEN_SUPER);
  case 't':
    if (scanner.lexeme_current - scanner.lexeme_start > 1) {
      switch (scanner.lexeme_start[1]) {
      case 'h':
        return check_identifier_keyword(2, 2, "is", TOKEN_THIS);
      case 'r':
        return check_identifier_keyword(2, 2, "ue", TOKEN_TRUE);
      }
    }
    break;
  case 'v':
    return check_identifier_keyword(1, 2, "ar", TOKEN_VAR);
  case 'w':
    return check_identifier_keyword(1, 4, "hile", TOKEN_WHILE);
  }

  return TOKEN_IDENTIFIER;
}

static bool is_eof() { return *scanner.lexeme_current == '\0'; }

static bool is_digit(char lexeme_character) {
  return lexeme_character >= '0' && lexeme_character <= '9';
}

static bool is_alphabetic(char lexeme_character) {
  return (lexeme_character >= 'a' && lexeme_character <= 'z') ||
         (lexeme_character >= 'A' && lexeme_character <= 'Z') ||
         lexeme_character == '_';
}

static bool match_token(char expected_token) {
  if (is_eof())
    return false;
  if (*scanner.lexeme_current != expected_token)
    return false;
  scanner.lexeme_current++;
  return true;
}

static char peek_lexeme() { return *scanner.lexeme_current; }

static char peek_lexeme_next() {
  if (is_eof())
    return '\0';
  return scanner.lexeme_current[1];
}

static char advance_lexeme() {
  scanner.lexeme_current++;
  return scanner.lexeme_current[-1];
}

static Token make_token(TokenType type) {
  Token token;
  token.type = type;
  token.lexeme_start = scanner.lexeme_start;
  token.lexeme_length = (size_t)(scanner.lexeme_current - scanner.lexeme_start);
  token.line_number = scanner.line_number;
  return token;
}

static Token make_token_error(const char *message) {
  Token token;
  token.type = TOKEN_ERROR;
  token.lexeme_start = message;
  token.lexeme_length = (size_t)strlen(message);
  token.line_number = scanner.line_number;
  return token;
}

static Token make_token_string() {
  while (peek_lexeme() != '"' && !is_eof()) {
    if (peek_lexeme() == '\n')
      scanner.line_number++;
    advance_lexeme();
  }
  if (is_eof())
    return make_token_error("Unterminated string.");
  advance_lexeme();
  return make_token(TOKEN_STRING);
}

static Token make_token_number() {
  while (is_digit(peek_lexeme()))
    advance_lexeme();
  if (peek_lexeme() == '.' && is_digit(peek_lexeme_next())) {
    advance_lexeme();
    while (is_digit(peek_lexeme()))
      advance_lexeme();
  }
  return make_token(TOKEN_NUMBER);
}

static Token make_token_identifier() {
  while (is_alphabetic(peek_lexeme()) || is_digit(peek_lexeme()))
    advance_lexeme();
  return make_token(get_identifier_type());
}

static void advance_whitespaces() {
  for (;;) {
    char lexeme_character = peek_lexeme();
    switch (lexeme_character) {
    case ' ':
    case '\r':
    case '\t':
      advance_lexeme();
      break;
    case '\n':
      scanner.line_number++;
      advance_lexeme();
      break;
    default:
      return;
    }
  }
}

static void advance_comments() {
  for (;;) {
    char lexeme_character = peek_lexeme();
    switch (lexeme_character) {
    case '/':
      if (peek_lexeme_next() == '/') {
        while (peek_lexeme() != '\n' && !is_eof())
          advance_lexeme();
      } else {
        return;
      }
      break;
    default:
      return;
    }
  }
}

void init_scanner(const char *input) {
  scanner.lexeme_start = input;
  scanner.lexeme_current = input;
  scanner.line_number = 1;
}

Token scan_token() {
  advance_whitespaces();
  advance_comments();

  scanner.lexeme_start = scanner.lexeme_current;
  if (is_eof())
    return make_token(TOKEN_EOF);

  char lexeme_character = advance_lexeme();

  if (is_alphabetic(lexeme_character))
    return make_token_identifier();
  if (is_digit(lexeme_character))
    return make_token_number();

  switch (lexeme_character) {
  case '(':
    return make_token(TOKEN_LEFT_PARENTHESIS);
  case ')':
    return make_token(TOKEN_RIGHT_PARENTHESIS);
  case '{':
    return make_token(TOKEN_LEFT_BRACE);
  case '}':
    return make_token(TOKEN_RIGHT_BRACE);
  case ';':
    return make_token(TOKEN_SEMICOLON);
  case ',':
    return make_token(TOKEN_COMMA);
  case '.':
    return make_token(TOKEN_DOT);
  case '-':
    return make_token(TOKEN_MINUS);
  case '+':
    return make_token(TOKEN_PLUS);
  case '/':
    return make_token(TOKEN_SLASH);
  case '*':
    return make_token(TOKEN_STAR);
  case '!':
    return make_token(match_token('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
  case '=':
    return make_token(match_token('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
  case '<':
    return make_token(match_token('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
  case '>':
    return make_token(match_token('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
  case '"':
    return make_token_string();
  }

  return make_token_error("Unrecognized token.");
}
